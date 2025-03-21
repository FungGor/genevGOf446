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
	int32_t driverTemperature;
}MOTOR;

#define SAMPLE LENGTH               128u
#define MOTOR_OVERTEMP_THRESHOLD      90
#define DRIVER_OVERTEMP_THRESHOLD     70
#define LOW_BATTERY_THRESHOLD         36

extern void motor_param_init();

extern void motor_speed();

extern void motor_current();

extern void motor_rms_current();

extern void setDCVoltage(uint32_t voltage);

extern uint32_t getDCVoltage();

extern bool underVoltage();

extern void setMotorTemperature(int32_t temperature);

extern int32_t getMotorTemperature();

extern bool MotorOverTemperature();

extern void setDriverTemperature(int32_t temperature);

extern int32_t getDriverTemperature();

extern bool DriverOverTemperature();

extern bool isStop();

extern int16_t getRPM();

extern void setMOTORPower(int32_t powerSet);

extern int32_t getMOTORPower();

extern void getIqIdMotor();

extern void calcDC();

extern int32_t getDC();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_MOTOR_PARAM_H_ */
