/*
 * Drive.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

/***********************************************ESCOOTER TRANSMISSION UNITS (ETU)*********************************************/

#include <backgroundTask.h>
#include "ETU_OBD.h"
#include "ETU_StateMachine.h"
#include "escooter_transmission_units.h"
#include "SleepAndWake.h"
#include "brake_and_throttle.h"
#include "battery_current_sensors.h"
#include "tail_light.h"
#include "PROTOCOL_HANDLER.h"
#include "ERROR_REPORT.h"
#include "flash_internal.h"
#include "UDHAL_TIMEOUT.h"
#include "UDHAL_MOTOR.h"
#include "motor_param.h"
#include "hardwareParam.h"
#include "mc_tasks.h"
#include "cmsis_os.h"

static osThreadId driveHandle;
static osPriority priority;

static ETU_StateHandle_t ETU_State;

static uint8_t N1_ticks = N1_TIME / GeneralTask_TIME;
static uint8_t N2_ticks = N2_TIME / GeneralTask_TIME;
static uint8_t N3_ticks = N3_TIME / GeneralTask_TIME;


//tail light variables
static uint32_t taskSleepCount = 0;  //2025-05-12
static uint8_t ledIndicatorStatus = 0;

static bool *ptr_drive_POWER_ON;
static uint8_t *ptr_error_report;       /*MOTOR CONTROLLER HARDWARE ERROR*/
static uint8_t *software_error_report;  /*SOFTWARE ERROR*/
static bool *ptrOBDflag;
static uint8_t isPowerByBattery = 0x00;
static uint8_t ETU_Start[2];

static void GeneralTasks(void const * argument);

void go_powerOnRegister(bool *ptrpowerOn)
{
	ptr_drive_POWER_ON = ptrpowerOn;
}


void go_errorReportRegister(uint8_t *report)
{
	ptr_error_report = report;
}

void software_errorReportRegister(uint8_t *fault)
{
	software_error_report = fault;
}

void OBD_flagRegister(bool *ptrOBD)
{
	ptrOBDflag = ptrOBD;
}

/* Safety Power Cutting 2025-03-25 */
void ETU_PowerShutDown()
{
	Motor_ShutDown();
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
	/*Idle Task*/
	powerManagementInit();
    /*UART Connection Status is also monitored in Background Task*/
	BACKGROUND_CONNECTION_MONITOR_INIT();
	/*Idle Task*/
	run_background_tasks();
	/*It is the main task for controlling the E-Scooter*/
	createDrivingTasks();
	ETU_Init(&ETU_State);
	etu_state_ptr_register(&ETU_State);
}

void createDrivingTasks(void)
{
	/*Starts Driving Task*/
	osThreadDef(drive, GeneralTasks,osPriorityBelowNormal,0,128);
	driveHandle = osThreadCreate(osThread(drive), NULL);
}

void ETU_BootRoutine(uint8_t diagnosis)
{
	if(ETU_GetState(&ETU_State) == BOOT_CHECK)
	{
		if(getBootSource() == CONNECT_BATTERY) isPowerByBattery = 0x01;

		if(isPowerByBattery == 0x00)
		{
			/* ETU doesn't go into OBD Mode when the E-Scooter is operating, to enter into OBD
			 * Backdoor command should be given externally, i.e. software means
			 */
			if(diagnosis == 0x00) { ETU_NextState(&ETU_State,ETU_START);}
			else if(diagnosis == 0x01){ ETU_NextState(&ETU_State,ETU_OBD);}
		}
		else if(isPowerByBattery == 0x01)
		{
			ETU_NextState(&ETU_State,ETU_OFF_TRANSITION);
		}
	}
}


void gearReady()
{
   /*  Make a bit array/register. To ready for driving (D-Mode), the following conditions should be met:
    *  https://www.cs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
    *  1. *ptr_error_report == 0  --> gear is ready
    *  2. *software_error_report == 0 --> gear is ready
    *  3.  --> If Iq = 0 / ptr_brakeAndThrottle.throttleTriggered = false --> gear is not ready; If Iq != 0 / ptr_brakeAndThrottle.throttleTriggered = true --> gear is ready
    *  If any of bits in GearReady[] register is 0, the gear is not ready yet, throttleSignalInput() is not triggered --> go back to N (Neutral) mode.
    * */
}

/*This is the main task for E-Scooter Transmission Unit  (ETU)
 *Why it drains a lots in the battery for the E-Scooter!
 * */
static void GeneralTasks(void const * argument)
{
	priority = osThreadGetPriority(NULL);

	ETU_BootRoutine(SKIP_DIAGNOSIS);

	if(ETU_GetState(&ETU_State) == ETU_START)
	{
	   updateConnectionStatus(false,0);
	   timeOutStart();
	   for(;;)
	   {
		   osDelay(GeneralTask_TIME);
		   /****************  Task timing & sleep *******************/
		   /* Task sleep must be positioned at the beginning of the for loop */
		   if(*ptr_drive_POWER_ON == true)
		   {
			  if(ledIndicatorStatus == 0)
			  {
				 ledIndicatorStatus = led_indicator_on();
			  }
	       /***************************************************************
	       * N1_TIME = 100 ms
	       *
	       * if GeneralTasks_TIME = 0.02 seconds, N1_TIME = 0.100 seconds, then N_1 = 5
	       * The following loop is refreshed/executed every N_1 loops
	       *
	       * Regularly checks ETU Status
	       **************************************************************/
		     if((taskSleepCount % N1_ticks) == 0)
		     {
			   /*Read Motor Parameters*/
			   getIqIdMotor();
			   /*Check DC Current*/
			   calcDC();
			   /*Sends Motor Fault Report to the Dash-board*/
			   CHECK_MOTOR_STATUS();
			   /*Is the throttle twisted ?*/
			   refreshThrottleStatus();
			   /*Tail Light Control*/
			   lightSensorStateChange();
			   /*Monitor Motor Speed*/
			   motor_speed();
			   /*Motor Root Mean Square Current*/
			   motor_rms_current();
		     }
	       /***************************************************************
	        * N2_TIME = 200 ms
	        *
	        * if GeneralTasks_TIME = 0.02 seconds, N2_TIME = 0.200 seconds, then N_2 = 10
	        * The following loop is refreshed/executed every N_2 loops
	        * --> Toggle The Tail Light
	        **************************************************************/
		     if((taskSleepCount % N2_ticks) == 0)
		     {
		    	 tailLightStateMachine();
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
				checkConnectionStatus();
		     }

		     if(*software_error_report != 0x00)
		     {
				ETU_PowerShutDown();
				ledIndicatorStatus = led_indicator_off();
				set_tail_light_off();  //2025-05-03
				error_indicator_off(); //2025-05-03
				timeOutStop();         //2025-05-03
				ETU_NextState(&ETU_State,ETU_OFF);
		    	break;
		     }


		     /*Might Be replaced by void engineReady() */
			 if(*ptr_error_report == 0x00)
			 {
			    throttleSignalInput();
			 }
			 else if(*ptr_error_report != 0x00)            /*Usually comes from the hardware's fatal errors */
			 {
				setIQ(0);
				set_ThrottlePercent(0);
				throttleSignalInput();
				driveStop(); /*Neutral Gear*/
				//ETU_PowerShutDown();
				/*Turn Off the Motor Controller after stopping the motor*/
				error_indicator_on();
				//timeOutStop();
				/*Toggle to the next ETU State: ETU_FAULT*/
				//ETU_NextState(&ETU_State,ETU_FAULT);
				//break;
			 }
		 }
		 else if(*ptr_drive_POWER_ON == false)
		 {
			ETU_PowerShutDown();
			ledIndicatorStatus = led_indicator_off();
			set_tail_light_off();
			error_indicator_off();
			timeOutStop();
			ETU_NextState(&ETU_State,ETU_OFF);
			break;
			/*Try to create a new RTOS Task which handles LOW POWER MODE*/
		 }
		taskSleepCount++;
		if(taskSleepCount == UINT32_MAX)taskSleepCount = 0;
	  }
	}


	if(ETU_GetState(&ETU_State) == ETU_OFF_TRANSITION )
	{
		for(;;)
		{
			error_indicator_on();
			*ptr_drive_POWER_ON = false;
			if(*ptr_drive_POWER_ON == false)
			{
				ETU_PowerShutDown();
				ledIndicatorStatus = led_indicator_off();
				set_tail_light_off();
				error_indicator_off();
				ETU_NextState(&ETU_State,ETU_OFF);
				break;
			}
		}
	}


	/*Shut Down Process Begins*/
	if(ETU_GetState(&ETU_State) == ETU_OFF || ETU_GetState(&ETU_State) == ETU_OBD ||
	   ETU_GetState(&ETU_State) == ETU_FAULT)
	{
		/* A function that implements a task must not exit or attempt to return to
			its caller as there is nothing to return to.  If a task wants to exit it
			should instead call vTaskDelete( NULL ).

			Artificially force an assert() to be triggered if configASSERT() is
			defined, then stop here so application writers can catch the error. static void prvTaskExitError() from port.c*/
		vTaskDelete(NULL); //Kills the task !
	}
}
