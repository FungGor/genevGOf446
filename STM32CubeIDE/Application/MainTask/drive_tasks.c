/*
 * Drive.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */
#include "drive_tasks.h"
#include "SleepAndWake.h"
#include "brake_and_throttle.h"
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
	tail_light_off();
	/*Handles transmission timeout when the connection is lost*/
	UDHAL_TIMEOUT_init();
	/*It is the main task for controlling the E-Scooter*/
	createDrivingTasks();
}

void createDrivingTasks(void)
{
	/*Starts Driving Task*/
	osThreadDef(drive, GeneralTasks,osPriorityBelowNormal,0,128);
	driveHandle = osThreadCreate(osThread(drive), NULL);
}

/*MAIN TASKS*/
uint8_t needPowerOff = 0x00;
void GeneralTasks(void const * argument)
{
	priority = osThreadGetPriority(NULL);
	led_indicator_on();
	updateConnectionStatus(false,0);
	timeOutStart();
	for(;;)
	{
		/*Sends Motor Fault Report to the Dash-board*/
		CHECK_MOTOR_STATUS();
		/*Is the throttle twisted ?*/
		refreshThrottleStatus();
		/*Is the brake pressed ?
		 *If Yes -> Give an alarm signal !
		 *If No -> Turn off the tail light ! */
		brakeStateChange();
		/*Is the environment dark?
		 *  If Yes -> turn on the tail light !
		 *  If No  -> turn off the tail light !*/
		lightSensorStateChange();
		/*Get Iq values to control the motor*/
		get_ThrottleInformation();
        /*Connection lost?*/
		checkConnectionStatus();

		motor_speed();

		motor_current();

		if( ((*software_error_report == 0x00) || (*ptr_error_report == 0x00)) )
		{
			throttleSignalInput();
		}
		else if( (*software_error_report != 0x00) || (*ptr_error_report != 0x00))
		{
			setIQ(0);
			set_ThrottlePercent(0);
			throttleSignalInput();
			driveStop();
			/*Turn Off the Motor Controller after stopping the motor*/
			error_indicator_on();
		}

		if(getPowerMode() == false)
		{
			needPowerOff = 0x01;
			/*Jump to shut-down process*/
			break;
		}
	}
	/*Shut Down Process Begins*/
	gotoSLEEP();
}

void ES_SafetyTask()
{
	accelerateIQMotor(0,0);
	motorStop();
}
