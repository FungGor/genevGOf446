/*
 * health_check.h
 *
 *  Created on: Apr 25, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_BOOT_CHECK_HEALTH_CHECK_H_
#define APPLICATION_BOOT_CHECK_HEALTH_CHECK_H_
#ifdef __cplusplus
extern "C"{
#endif
#include "stdint.h"
#include "stdbool.h"

typedef struct
{
	uint8_t VoltageFail;
	uint8_t SpeedSensorFail;
	uint8_t MotorTempFail;
	uint8_t DriverTempFail;
	uint8_t HealthCheckFail;;
}Health_Check_Handler_t;

extern void HealthCheckInit();

extern uint8_t CheckBusVoltage();

extern uint8_t CheckSpeedSensor();

extern uint8_t CheckCurrentSensor();

extern uint8_t CheckMotorTemperature();

extern uint8_t CheckMotorDriverTemperature();

extern uint8_t DiagnosisFault();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_BOOT_CHECK_HEALTH_CHECK_H_ */
