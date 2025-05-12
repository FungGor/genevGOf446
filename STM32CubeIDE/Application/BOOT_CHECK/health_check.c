/*
 * health_check.c
 *
 *  Created on: Apr 25, 2025
 *      Author: TerenceLeung
 */
#include "health_check.h"
#include "flash_internal.h"
#include "motor_param.h"
#include "mc_tasks.h"
#include "hall_speed_pos_fdbk.h"
#include "ERROR_REPORT.h"

Health_Check_Handler_t healthStatus;
void HealthCheckInit()
{
	healthStatus.VoltageFail      = 0x00;
	healthStatus.SpeedSensorFail  = 0x00;
	healthStatus.MotorTempFail    = 0x00;
	healthStatus.DriverTempFail   = 0x00;
	healthStatus.HealthCheckFail  = 0x00;
}

uint8_t CheckBusVoltage()
{
	healthStatus.VoltageFail = 0x00;
	if(underVoltage() == true)
	{
		healthStatus.VoltageFail = 0x01;
	}
	else if(overVoltage() == true)
	{
		healthStatus.VoltageFail = 0x02;
	}
	return healthStatus.VoltageFail;
}

uint8_t CheckSpeedSensor()
{
	if(HallSensorFail() == true)
	{
		healthStatus.SpeedSensorFail = 0x01;
	}
	else
	{
		healthStatus.SpeedSensorFail = 0x00;
	}
	return healthStatus.SpeedSensorFail;
}

uint8_t CheckCurrentSensor()
{
	uint8_t checkFault = 0xFF;
	return checkFault;
}

uint8_t CheckMotorTemperature()
{
	if (isMotorTemperatureAbnormal() == true)
	{
		healthStatus.MotorTempFail = 0x01;
	}
	else
	{
		healthStatus.MotorTempFail = 0x00;
	}
	return healthStatus.MotorTempFail;
}

uint8_t CheckMotorDriverTemperature()
{
	if(DriverOverTemperature() == true)
	{
		healthStatus.DriverTempFail = 0x01;
	}
	else
	{
		healthStatus.DriverTempFail = 0x00;
	}
	return healthStatus.DriverTempFail;
}

uint8_t DiagnosisFault()
{
	uint8_t ErrorMask = 0x00;
	ErrorMask = CheckBusVoltage() | CheckSpeedSensor() | CheckMotorDriverTemperature() ;
	if (ErrorMask != 0x00)
	{
		healthStatus.HealthCheckFail = 0x01;
	}
	else if(ErrorMask == 0x00)
	{
		healthStatus.HealthCheckFail = 0x00;
	}
	return healthStatus.HealthCheckFail;
}
