/*
 * motor_param.h
 *
 *  Created on: Jan 21, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_MOTOR_PARAM_H_
#define APPLICATION_HARDWARE_MOTOR_PARAM_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"

typedef struct
{
	int16_t  SPEED;
	float CURRENT;
	int16_t VOLTAGE;
	float RMS_CURRENT;
}MOTOR;

void motor_param_init();

void motor_speed();

void motor_current();

void motor_rms_current();

void motor_voltage();

bool isStop();

int16_t getRPM();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_MOTOR_PARAM_H_ */
