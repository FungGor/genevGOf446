/*
 * UDHAL_MOTOR.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef UDHAL_UDHAL_MOTOR_H_
#define UDHAL_UDHAL_MOTOR_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

extern void accelerateIQMotor(int16_t torque, uint16_t ramp);

extern void accelerateSpeedMotor(int16_t targetSpeed, uint16_t ramp);

extern void motorStop();

extern void ackErrorDebug();

int16_t getSpeed();

float getCurrent();

float getRMSCurrent();

int16_t getVoltage();

int16_t getRotorPosition();


#ifdef __cplusplus
}
#endif
#endif /* UDHAL_UDHAL_MOTOR_H_ */
