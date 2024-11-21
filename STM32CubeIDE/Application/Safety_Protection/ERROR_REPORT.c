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

void ERROR_REPORT_INIT()
{
	go_errorReportRegister(&ERROR_OCCURRED);
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
     SEND_ERROR_REPORT(status);
}

void SEND_ERROR_REPORT(uint16_t error_code)
{
    ERROR_OCCURRED = error_code;
}

uint8_t GET_ERROR_REPORT()
{
	if(ERROR_OCCURRED == MC_NO_ERROR)
	{
		ERROR_CODE = SYSTEM_NORMAL;
	}
	else if(ERROR_OCCURRED == MC_SPEED_FDBK)
	{
		ERROR_CODE = HALL_SENSOR_ERROR_CODE;
	}
	else if(ERROR_OCCURRED == MC_OVER_VOLT || ERROR_OCCURRED == MC_UNDER_VOLT)
	{
		ERROR_CODE = ABNORMAL_CURRENT;
	}
	else if(ERROR_OCCURRED == MC_OVER_TEMP)
	{
		ERROR_CODE  = BATTERY_TEMP_ERROR_CODE;
	}
	else if(ERROR_OCCURRED == TRANSMISSION_TIMEOUT)
	{
		ERROR_CODE = TRANSMISSION_TIMEOUT;
	}
	return ERROR_CODE;
}
