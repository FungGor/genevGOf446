/*
 * ERROR_CHECK.c
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */
#include <ERROR_REPORT.h>
#include "mc_type.h"
#include "mc_api.h"
#include "drive_tasks.h"
#include "main.h"

/*System is normal in default*/
uint8_t ERROR_OCCURRED = MC_NO_ERROR;
uint8_t ERROR_CODE = SYSTEM_NORMAL;
uint8_t SOFTWARE_ERROR = SOFTWARE_OK;

void ERROR_REPORT_INIT()
{
	go_errorReportRegister(&ERROR_OCCURRED);
}

void SOFTWARE_ERROR_REPORT_INIT()
{
	software_errorReportRegister(&SOFTWARE_ERROR);
}
/*********************************************************************
 * @fn      CHECK_MOTOR_STATUS
 *
 * @brief   This function must be called frequently to check any motor faults/failures
 *
 * @param   None
 *
 * @return  None
 */
void CHECK_MOTOR_STATUS()
{
     uint16_t status = MC_GetOccurredFaultsMotor1();
     SEND_MOTOR_ERROR_REPORT(status);
}

void SEND_MOTOR_ERROR_REPORT(uint16_t error_code)
{
    ERROR_OCCURRED = (uint8_t)error_code;
}

void SEND_SOFTWARE_ERROR_REPORT(uint8_t fault)
{
	SOFTWARE_ERROR = fault;
}

uint8_t GET_MOTOR_ERROR_REPORT()
{
	if(ERROR_OCCURRED == MC_NO_ERROR)
	{
		ERROR_CODE = SYSTEM_NORMAL;
	}
	else if(ERROR_OCCURRED == MC_SPEED_FDBK)
	{
		ERROR_CODE = HALL_SENSOR_ERROR_CODE;
	}
	else if(ERROR_OCCURRED == MC_UNDER_VOLT)
	{
		ERROR_CODE = ABNORMAL_CURRENT;
	}
	else if(ERROR_OCCURRED == MC_OVER_TEMP)
	{
		ERROR_CODE  = BATTERY_TEMP_ERROR_CODE;
	}
	return ERROR_CODE;
}

uint8_t GET_SOFTWARE_ERROR_REPORT()
{
	return SOFTWARE_ERROR;
}
