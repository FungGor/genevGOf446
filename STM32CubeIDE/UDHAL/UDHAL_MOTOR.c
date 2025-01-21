/*
 * UDHAL_MOTOR.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#include "../UDHAL/UDHAL_MOTOR.h"

#include "mc_api.h"
#include "mc_type.h"
#include "hardwareParam.h"

void accelerateIQMotor(int16_t torque, uint16_t ramp)
{
	MC_ProgramTorqueRampMotor1(torque,ramp);
	MC_StartMotor1();
}

void accelerateSpeedMotor(int16_t targetSpeed, uint16_t ramp)
{
	MC_ProgramSpeedRampMotor1(targetSpeed, ramp);
	MC_StartMotor1();
}

void motorStop()
{
	MC_StopMotor1();
}

extern void ackErrorDebug()
{
	MC_AcknowledgeFaultMotor1();
}

int16_t getSpeed()
{
	/*Returns the speed in RPM*/
	int16_t MOTOR_SPEED = (MC_GetMecSpeedAverageMotor1() * _RPM) / SPEED_UNIT;
	return MOTOR_SPEED;
}

float getCurrent()
{
	float MOTOR_CURRENT = (MC_GetPhaseCurrentAmplitudeMotor1() * VDD_SUPPLY)/(ADC_RANGE*CURRENT_SENSOR_GAIN);
	return MOTOR_CURRENT;
}

int16_t getVoltage()
{
	int16_t MOTOR_VOLTAGE = MC_GetPhaseVoltageAmplitudeMotor1();
	return MOTOR_VOLTAGE;
}

void motorAnalysis()
{

}
