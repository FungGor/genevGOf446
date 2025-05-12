/*
 * brake_and_throttle.c
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */

#include "brake_and_throttle.h"
#include "UDHAL_MOTOR.h"
#include "ETU_OBD.h"

brakeAndThrottle_t ptr_brakeAndThrottle;
int16_t torque = 0;

void brake_and_throttle_init()
{
	ptr_brakeAndThrottle.IQ_applied = 0;
	ptr_brakeAndThrottle.throttlePercent = 0;
	ptr_brakeAndThrottle.throttleTriggered = false;
	ptr_brakeAndThrottle.brakeTriggered   = false;
	ptr_brakeAndThrottle.allowable_rpm = 0;
	ptr_brakeAndThrottle.speed_mode_IQmax = 0;
	ptr_brakeAndThrottle.ramp_rate  = 0;
	ptr_brakeAndThrottle.throttle_voltage = 0;
	ETU__brakeThrottleSensorRegister(&ptr_brakeAndThrottle);
}

void setIQ(int16_t IQ)
{
	ptr_brakeAndThrottle.IQ_applied = IQ;
}

int16_t getIQ()
{
	return ptr_brakeAndThrottle.IQ_applied;
}

void set_ThrottlePercent(uint16_t percentage)
{
	ptr_brakeAndThrottle.throttlePercent = percentage;
}

uint16_t getThrottlePercent()
{
	return ptr_brakeAndThrottle.throttlePercent;
}

void throttleSignalInput()
{
	accelerateIQMotor(ptr_brakeAndThrottle.IQ_applied,0);
#ifdef MOTOR_CONTROL
	accelerateIQMotor(ptr_brakeAndThrottle->IQ_applied,0);
#endif
}

void refreshThrottleStatus()
{
	if (getIQ() == 0)
	{
		ptr_brakeAndThrottle.throttleTriggered = false;
	}
	else if(getIQ() > 0)
	{
		ptr_brakeAndThrottle.throttleTriggered = true;
	}
}

bool getThrottleStatus()
{
	return ptr_brakeAndThrottle.throttleTriggered;
}

void setThrottle_Voltage(uint16_t percentage)
{
	float throttleADCAvg = ((percentage/100)*(THROTTLE_ADC_CALIBRATE_H+THROTTLE_ADC_CALIBRATE_L)) + THROTTLE_ADC_CALIBRATE_L;
	ptr_brakeAndThrottle.throttle_voltage = (uint16_t)throttleADCAvg;
}

uint16_t getThrottle_Voltage()
{
	return ptr_brakeAndThrottle.throttle_voltage;
}

void updateBrakeStatus(bool status)
{
	ptr_brakeAndThrottle.brakeTriggered = status;
}

bool getBrakeStatus()
{
	return ptr_brakeAndThrottle.brakeTriggered;
}

void changeSpeedMode(int16_t speed_mode_IQmax, int16_t allowable_rpm, uint16_t ramp_rate)
{
	ptr_brakeAndThrottle.speed_mode_IQmax = speed_mode_IQmax;
	ptr_brakeAndThrottle.allowable_rpm    = allowable_rpm;
	ptr_brakeAndThrottle.ramp_rate        = ramp_rate;
}

int16_t Iq_applied = 0;
uint16_t Iq_Percentage = 0;
void get_ThrottleInformation()
{
    Iq_applied = getIQ();
    Iq_Percentage = getThrottlePercent();
}

void driveStop()
{
	motorStop();
}
