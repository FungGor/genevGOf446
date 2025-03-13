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
#include "mc_type.h"
#include "mc_api.h"

typedef struct
{
	int16_t  SPEED;
	int32_t CURRENT;
	uint32_t VBUS;
	float RMS_CURRENT;
	int32_t POWERmW;
	int32_t Iq;
	int32_t Id;
	uint16_t milliAmpere;
	int32_t motorTemperature;
}MOTOR;

#define SAMPLE LENGTH          128u

void motor_param_init();

void motor_speed();

void motor_current();

void motor_rms_current();

void setDCVoltage(uint32_t voltage);

uint32_t getDCVoltage();

void setMotorTemperature(int32_t temperature);

int32_t getMotorTemperature();

bool isStop();

int16_t getRPM();

void setMOTORPower(int32_t powerSet);

int32_t getMOTORPower();

void getIqIdMotor();

void calcDC();

int32_t getDC();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_MOTOR_PARAM_H_ */
