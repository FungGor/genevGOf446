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
#include "cmsis_os.h"

osThreadId driveHandle;
osPriority priority;

static bool *ptr_drive_POWER_ON;
static uint8_t *ptr_error_report;

void go_powerOnRegister(bool *ptrpowerOn)
{
	ptr_drive_POWER_ON = ptrpowerOn;
}

bool getPowerMode()
{
	if(*ptr_drive_POWER_ON == true)
	{
		return (true);
	}
	else if(*ptr_drive_POWER_ON == false)
	{
		return (false);
	}
	return (true);
}

void go_errorReportRegister(uint8_t *report)
{
	ptr_error_report = report;
}

void GoInit()
{
	/*Monitors the motor driver's power status*/
	powerModeInit();
	/*Reports existing errors to the Dash-Board constantly*/
	ERROR_REPORT_INIT();
	brake_and_throttle_init();
	tail_light_off();
	/*Handles transmission timeout when the connection is lost*/
	UDHAL_TIMEOUT_init();
	/*It is the main task for controlling the E-Scooter*/
	createDrivingTasks();
}

void createDrivingTasks(void)
{
	/*Starts Driving Task*/
	osThreadDef(drive, ES_RunDrivingTasks,osPriorityBelowNormal,0,128);
	driveHandle = osThreadCreate(osThread(drive), NULL);
}

void ES_RunDrivingTasks(void const * argument)
{
	priority = osThreadGetPriority(NULL);
	timeOutStart();
	for(;;)
	{
		CHECK_MOTOR_STATUS();
		refreshThrottleStatus();
		lightStateChange();

		if(*ptr_error_report == 0x00)
		{
			if(getThrottleStatus() == true)
			{
				//throttleSignalInput();
			}
			else if( (getThrottleStatus() == false) || (getSpeed() <= 10) )
			{
				//ES_SafetyTask();
			}
		}else if(*ptr_error_report != 0x00)
		{
			//ES_SafetyTask();
			break; /*Please restart again*/
		}

		if(getPowerMode() == false)
		{
			break;
		}
	}
	/*Shut Down Process Begins*/
}

void ES_SafetyTask()
{
	accelerateIQMotor(0,0);
	motorStop();
}
