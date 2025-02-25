/*
 * motor_param.c
 *
 *  Created on: Jan 21, 2025
 *      Author: TerenceLeung
 */

#include "motor_param.h"
#include "UDHAL_MOTOR.h"

MOTOR ptrMotor;

void motor_param_init()
{
	ptrMotor.SPEED = 0;
	ptrMotor.CURRENT = 0;
	ptrMotor.VBUS = 0;
	ptrMotor.POWERmW = 0;
	ptrMotor.Iq = 0;
	ptrMotor.Id = 0;
	ptrMotor.milliAmpere = 0;
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
	ptrMotor.Iq = torqueCurrent.q;
	ptrMotor.Id = torqueCurrent.d;
	//Total Torque = sqrt(Iq*Iq + Id*Id)
}

void calcDC()
{
	ptrMotor.milliAmpere = ptrMotor.POWERmW / ptrMotor.VBUS;
	/*Apply Moving average filtering (Optional)*/
}

int32_t getDC()
{
	return ptrMotor.milliAmpere;
}
