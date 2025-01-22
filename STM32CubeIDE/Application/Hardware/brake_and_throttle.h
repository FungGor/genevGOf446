/*
 * brake_and_throttle.h
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_BRAKE_AND_THROTTLE_H_
#define APPLICATION_HARDWARE_BRAKE_AND_THROTTLE_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"
#include "hardwareParam.h"

typedef struct
{
	int16_t IQ_applied;
	uint16_t throttlePercent;
	bool throttleTriggered;
	bool brakeTriggered;
	int16_t allowable_rpm;
	int16_t speed_mode_IQmax;
	uint16_t ramp_rate;
}brakeAndThrottle_t;

void brake_and_throttle_init();

void setIQ(int16_t IQ);

int16_t getIQ();

void set_ThrottlePercent(uint16_t percentage);

uint16_t getThrottlePercent();

void throttleSignalInput();

void refreshThrottleStatus();

bool getThrottleStatus();

void updateBrakeStatus(bool status);

bool getBrakeStatus();

void changeSpeedMode(int16_t speed_mode_IQmax, int16_t allowable_rpm, uint16_t ramp_rate);

void get_ThrottleInformation();

void driveStop();
#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_BRAKE_AND_THROTTLE_H_ */
