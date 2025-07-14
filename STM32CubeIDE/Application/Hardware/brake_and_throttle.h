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


#define THROTTLE_ADC_CALIBRATE_L     850
#define THROTTLE_ADC_CALIBRATE_H     2200
#define BRAKE_ADC_CALIBRATE_L        830
#define BRAKE_ADC_CALIBRATE_H        2200

typedef struct
{
	int16_t IQ_applied;
	uint16_t throttlePercent;
	bool throttleTriggered;
	bool brakeTriggered;
	int16_t allowable_rpm;
	int16_t speed_mode_IQmax;
	uint16_t ramp_rate;
	uint16_t throttle_voltage;
}brakeAndThrottle_t;

extern void brake_and_throttle_init();

extern void setIQ(int16_t IQ);

extern int16_t getIQ();

extern void set_ThrottlePercent(uint16_t percentage);

extern uint16_t getThrottlePercent();

extern void throttleSignalInput();

extern bool getThrottleStatus();

extern void setThrottle_Voltage(uint16_t percentage);

extern uint16_t getThrottle_Voltage();

extern void updateBrakeStatus(bool status);

extern bool getBrakeStatus();

extern void changeSpeedMode(int16_t speed_mode_IQmax, int16_t allowable_rpm, uint16_t ramp_rate);

extern void driveStop();
#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_BRAKE_AND_THROTTLE_H_ */
