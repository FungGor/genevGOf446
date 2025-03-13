/*
 * Drive.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */
#include "drive_tasks.h"
#include "SleepAndWake.h"
#include "brake_and_throttle.h"
#include "battery_current_sensors.h"
#include "tail_light.h"
#include "PROTOCOL_HANDLER.h"
#include "ERROR_REPORT.h"
#include "UDHAL_TIMEOUT.h"
#include "UDHAL_MOTOR.h"
#include "motor_param.h"
#include "hardwareParam.h"
#include "cmsis_os.h"

osThreadId driveHandle;
osPriority priority;

osThreadId shutDownHandle;

uint8_t N1_ticks = 0;
uint8_t N2_ticks = 0;
uint8_t N3_ticks = 0;


//tail light variables
uint8_t tail_light_status_old = 0;      //either: 1 = 0N    or   0 = OFF
uint8_t tail_light_mode = ESCOOTER_TAIL_LIGHT_OFF;   // toggle = 0x05, ON = 0x08, OFF = 0x06
uint8_t tail_light_mode_old = ESCOOTER_TAIL_LIGHT_OFF; // toggle = 0x05, ON = 0x08, OFF = 0x06
uint8_t behaviourID_old = ESCOOTER_TAIL_LIGHT_OFF;
uint16_t taskSleepCount = 0;
uint8_t ledIndicatorStatus = 0;

static bool *ptr_drive_POWER_ON;
static uint8_t *ptr_error_report;       /*MOTOR CONTROLLER HARDWARE ERROR*/
static uint8_t *software_error_report;  /*SOFTWARE ERROR*/



void go_powerOnRegister(bool *ptrpowerOn)
{
	ptr_drive_POWER_ON = ptrpowerOn;
}

bool getPowerMode()
{
	if(*ptr_drive_POWER_ON == true)
	{
		return true;
	}
	else if(*ptr_drive_POWER_ON == false)
	{
		return false;
	}
	return true;
}

void go_errorReportRegister(uint8_t *report)
{
	ptr_error_report = report;
}

void software_errorReportRegister(uint8_t *fault)
{
	software_error_report = fault;
}

void GoInit()
{
	/*Peripheral Initialization*/
	/*Monitors the motor driver's power status*/
	powerModeInit();
	/*Reports existing errors to the Dash-Board constantly*/
	ERROR_REPORT_INIT();
	SOFTWARE_ERROR_REPORT_INIT();
	brake_and_throttle_init();
	motor_param_init();
	set_tail_light_off();
	/*Handles transmission timeout when the connection is lost*/
	UDHAL_TIMEOUT_init();
	/*It is the main task for controlling the E-Scooter*/
	createDrivingTasks();
	N1_ticks = N1_TIME / GeneralTask_TIME;
	N2_ticks = N2_TIME / GeneralTask_TIME;
	N3_ticks = N3_TIME / GeneralTask_TIME;
}

void createDrivingTasks(void)
{
	/*Starts Driving Task*/
	osThreadDef(drive, GeneralTasks,osPriorityBelowNormal,0,128);
	driveHandle = osThreadCreate(osThread(drive), NULL);
}

/*MAIN TASKS*/
int32_t DC_CURRENT = 0;
void GeneralTasks(void const * argument)
{
	priority = osThreadGetPriority(NULL);
	updateConnectionStatus(false,0);
	timeOutStart();
	for(;;)
	{
		osDelay(GeneralTask_TIME);
		/****************  Task timing & sleep *******************/
		/* Task sleep must be positioned at the beginning of the for loop */
		if(getPowerMode() == true)
		{
			if(ledIndicatorStatus == 0)
			{
				ledIndicatorStatus = led_indicator_on();
			}

			/*Sends Motor Fault Report to the Dash-board*/
			CHECK_MOTOR_STATUS();
			/*Is the throttle twisted ?*/
			refreshThrottleStatus();
			lightSensorStateChange();
			/*Get Iq values to control the motor*/
			get_ThrottleInformation();

			motor_speed();

			//motor_current();

			motor_rms_current();

	       /***************************************************************
	       * N1_TIME = 100 ms
	       *
	       * if GeneralTasks_TIME = 0.02 seconds, N1_TIME = 0.100 seconds, then N_1 = 5
	       * The following loop is refreshed/executed every N_1 loops
	       *
	       **************************************************************/
		   if((taskSleepCount % N1_ticks) == 0)
		   {
			   getIqIdMotor();
			   calcDC();
			   DC_CURRENT = getDC(); //Just for debugging the data type
		   }
	       /***************************************************************
	        * N2_TIME = 200 ms
	        *
	        * if GeneralTasks_TIME = 0.02 seconds, N2_TIME = 0.200 seconds, then N_2 = 10
	        * The following loop is refreshed/executed every N_2 loops
	        *
	        **************************************************************/
		    if((taskSleepCount % N2_ticks) == 0)
		    {
		    	tail_light_mode = get_tail_light_mode();
		    	if(tail_light_mode == ESCOOTER_TOGGLE_TAIL_LIGHT)
		    	{
		    		tail_light_status_old = toggle_tail_light(tail_light_status_old);
		    		tail_light_mode_old = ESCOOTER_TOGGLE_TAIL_LIGHT;
		    	}
		    	else
		    	{
		    		if(tail_light_mode_old != tail_light_mode)
		    		{
		    			if(tail_light_mode == ESCOOTER_TAIL_LIGHT_OFF)
		    			{
		    				set_tail_light_off();
		    				tail_light_status_old = get_tail_light_status(); // either: 1 = ON   or 0 = OFF CAUTION: THIS PARAMETER IS CONTROLLED BY UART
		    				tail_light_mode_old = ESCOOTER_TAIL_LIGHT_OFF;
		    			}
		    			else if(tail_light_mode == ESCOOTER_TAIL_LIGHT_ON)
		    			{
		    				set_tail_light_on();
		    				tail_light_status_old = get_tail_light_status(); // either: 1 = ON   or 0 = OFF CAUTION: THIS PARAMETER IS CONTROLLED BY UART
		    				tail_light_mode_old = ESCOOTER_TAIL_LIGHT_ON;
		    			}
		    			else
		    			{
		    				// shouldn't get here
		    			}
		    		}
		    	}
		    }
	        /***************************************************************
	         * N3_TIME = 500 ms
	         *
	         * if GeneralTasks_TIME = 0.02 seconds, N3_TIME = 0.500 seconds, then N_3 = 25
	         * The following loop is refreshed/executed every N_3 loops
	         *
	         **************************************************************/
		    if((taskSleepCount % N3_ticks) == 0)
		    {
		        /*Connection lost?*/
		    	//DC = BATTERYCURRENT_getRawCurrent();
				checkConnectionStatus();
				//showDC();
		    }

		    if(GET_SOFTWARE_ERROR_REPORT() != 0x00)
		    {
				setIQ(0);
				set_ThrottlePercent(0);
				throttleSignalInput();
				driveStop(); /*Neutral Gear*/
		    	error_indicator_on();
		    }


			if(*ptr_error_report == 0x00)
			{
				throttleSignalInput();
			}

			else if(*ptr_error_report != 0x00)
			{
				setIQ(0);
				set_ThrottlePercent(0);
				throttleSignalInput();
				driveStop(); /*Neutral Gear*/
				/*Turn Off the Motor Controller after stopping the motor*/
				error_indicator_on();
			}
		}
		else if(getPowerMode() == false)
		{
			setIQ(0);
			set_ThrottlePercent(0);
			throttleSignalInput();
			driveStop(); /*Ensure it is in Neutral Gear*/
			ledIndicatorStatus = led_indicator_off();
			set_tail_light_off();
			error_indicator_off();
			timeOutStop();
			break;
			/*Try to create a new RTOS Task which handles LOW POWER MODE*/
		}
		taskSleepCount++;
	}
	/*Shut Down Process Begins*/
	gotoSLEEP();
}

void ES_SafetyTask()
{
	accelerateIQMotor(0,0);
	motorStop();
}

