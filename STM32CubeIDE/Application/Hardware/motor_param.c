/*
 * motor_param.c
 *
 *  Created on: Jan 21, 2025
 *      Author: TerenceLeung
 */

#include "motor_param.h"
#include "UDHAL_MOTOR.h"
#include "ETU_OBD.h"
#include "math.h"

MOTOR ptrMotor;

void motor_param_init()
{
	ptrMotor.SPEED = 0;
	ptrMotor.CURRENT = 0;
	ptrMotor.VBUS = 0;
	ptrMotor.milliVolts = 0;
	ptrMotor.POWERmW = 0;
	ptrMotor.Iq = 0;
	ptrMotor.Id = 0;
	ptrMotor.milliAmpere = 0;
	ptrMotor.motorTemperature = 0;
	ptrMotor.driverTemperature = 0;
	ptrMotor.isMotorOverTemperature = false;
	ptrMotor.isDriverOverTemperature = false;
	ptrMotor.hallSensorState = 0;
	ptrMotor.electricAngle = 0;
	ETU_StatusRegister(&ptrMotor);
}

void motor_speed()
{
	ptrMotor.SPEED = getSpeed();
}

void motor_current()
{
	ptrMotor.CURRENT = getCurrent();
}

void motor_rms_current()
{
	ptrMotor.RMS_CURRENT = getRMSCurrent();

}

void setDCVoltage(uint32_t voltage)
{
	ptrMotor.VBUS = voltage;
}

uint32_t getDCVoltage()
{
	return ptrMotor.VBUS;
}

void setBatteryVoltage(int32_t milliVoltage)
{
	float conversionFactor = ADC_REFERENCE_VOLTAGE/VBUS_PARTITIONING_FACTOR;
	float milliVolts = milliVoltage*conversionFactor;
	milliVolts = milliVolts/65536;
	milliVolts = milliVolts*1000;
	/*Conversion Factor*/
	ptrMotor.milliVolts = milliVolts;
}

int32_t getBatteryVoltage()
{
	return ptrMotor.milliVolts;
}

bool underVoltage()
{
   if (ptrMotor.milliVolts < LOW_BATTERY_THRESHOLD)
   {
	   return true;
   }
   return false;
}

bool overVoltage()
{
	if(ptrMotor.milliVolts > OVER_VOLTAGE)
	{
		return true;
	}
	return false;
}

void setMotorTemperature(int32_t temperature)
{
	ptrMotor.motorTemperature = temperature;
}

int32_t getMotorTemperature()
{
	return ptrMotor.motorTemperature;
}

void updateMotorTemperatureStatus(bool status)
{
	ptrMotor.isMotorOverTemperature = status;
}

bool isMotorTemperatureAbnormal()
{
	if(ptrMotor.isMotorOverTemperature == true)
	{
		return true;
	}
	return false;
}

bool MotorOverTemperature()
{
	if (ptrMotor.motorTemperature > MOTOR_OVERTEMP_THRESHOLD)
	{
		return true;
	}
	return false;
}

void setDriverTemperature(int32_t temperature)
{
	ptrMotor.driverTemperature = temperature;
}

int32_t getDriverTemperature()
{
	return ptrMotor.driverTemperature;
}

void updateDriverTemperatureStatus(bool status)
{
	ptrMotor.isDriverOverTemperature = status;
}

bool DriverOverTemperature()
{
	if(ptrMotor.driverTemperature > DRIVER_OVERTEMP_THRESHOLD)
	{
		return true;
	}
	return false;
}

bool isStop()
{
	if(ptrMotor.SPEED == 0)
	{
		return true;
	}
	return false;
}

int16_t getRPM()
{
	return ptrMotor.SPEED;
}

void setMOTORPower(int32_t powerSet)
{
	ptrMotor.POWERmW = powerSet*1000;
}

int32_t getMOTORPower()
{
	return ptrMotor.POWERmW;
}

void getIqIdMotor()
{
	qd_t torqueCurrent;
	torqueCurrent = MC_GetIqdMotor1();
	ptrMotor.Iq = (int32_t)torqueCurrent.q;
	ptrMotor.Id = (int32_t)torqueCurrent.d;
}

void calcDC()
{
	ptrMotor.milliAmpere = ptrMotor.POWERmW / ptrMotor.VBUS;
	ptrMotor.milliAmpere = (uint16_t) ptrMotor.milliAmpere;
	/*Apply Moving average filtering (Optional)*/
}

int32_t getDC()
{
	return (int32_t)ptrMotor.milliAmpere;
}

void setHallState(uint8_t hall)
{
	ptrMotor.hallSensorState = hall;
}

void RotorAngle()
{
	ptrMotor.electricAngle = getRotorPosition();
}

int16_t getRotorAngle()
{
	return ptrMotor.electricAngle;
}
