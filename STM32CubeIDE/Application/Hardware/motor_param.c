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
	ptrMotor.VOLTAGE = 0;
}

void motor_speed()
{
	ptrMotor.SPEED = getSpeed();
}

void motor_current()
{
	ptrMotor.CURRENT = getCurrent();
}