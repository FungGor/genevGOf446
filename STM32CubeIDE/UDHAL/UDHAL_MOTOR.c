/*
 * UDHAL_MOTOR.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#include "../UDHAL/UDHAL_MOTOR.h"

#include "mc_api.h"
#include "mc_type.h"

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

int16_t getSpeed()
{
	/*Returns the speed in RPM*/
	int16_t MOTOR_SPEED = (MC_GetMecSpeedAverageMotor1() * _RPM) / SPEED_UNIT;
	return MOTOR_SPEED;
}

void motorAnalysis()
{

}
