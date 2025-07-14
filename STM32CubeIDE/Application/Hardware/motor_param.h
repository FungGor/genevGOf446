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
#include "parameters_conversion.h"

#define SAMPLE LENGTH                  128u
#define MOTOR_OVERTEMP_THRESHOLD         90
#define DRIVER_OVERTEMP_THRESHOLD        70
#define LOW_BATTERY_THRESHOLD         33000
#define OVER_VOLTAGE                  60000

typedef struct
{
	int16_t  SPEED;
	int32_t CURRENT;
	uint32_t VBUS;
	int32_t  milliVolts;
	float RMS_CURRENT;
	int32_t POWERmW;
	int32_t Iq;
	int32_t Id;
	uint16_t milliAmpere;
	int32_t motorTemperature;
	int32_t driverTemperature;
	bool isMotorOverTemperature;
	bool isDriverOverTemperature;
	uint8_t hallSensorState;
	int16_t electricAngle;
}MOTOR;

extern void motor_param_init();

extern void motor_speed();

extern void motor_current();

extern void motor_rms_current();

extern void setDCVoltage(uint32_t voltage);

extern uint32_t getDCVoltage();

extern void setBatteryVoltage(int32_t milliVoltage);

extern int32_t getBatteryVoltage();

extern bool underVoltage();

extern bool overVoltage();

extern void setMotorTemperature(int32_t temperature);

extern int32_t getMotorTemperature();

extern void updateMotorTemperatureStatus(bool status);

extern bool isMotorTemperatureAbnormal();

extern bool MotorOverTemperature();

extern void setDriverTemperature(int32_t temperature);

extern int32_t getDriverTemperature();

extern void updateDriverTemperatureStatus(bool status);

extern bool DriverOverTemperature();

extern bool isStop();

extern int16_t getRPM();

extern void setMOTORPower(int32_t powerSet);

extern int32_t getMOTORPower();

extern void getIqIdMotor();

extern void calcDC();

extern int32_t getDC();

extern void setHallState(uint8_t hall);

extern uint8_t getHallStatus();

extern void RotorAngle();

extern int16_t getRotorAngle();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_MOTOR_PARAM_H_ */
