/*
 * battery_manager.h
 *
 *  Created on: Jul 18, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_BATTERY_MANAGER_H_
#define APPLICATION_HARDWARE_BATTERY_MANAGER_H_

#ifdef __cplusplus
extern "C"{
#endif
#include "motor_param.h"

#define  VOLTAGE_SAMPLES               10
#define  DELTA_THRESHOLD_CHARGING     400
#define  DELTA_THRESHOLD_DISCHARGING  400

/*             Battery Charging Service Manager
 * 1. Regularly checks the battery voltage with 10 raw samples
 * 2. Calculates delta while checking the battery voltage
 * 3. Store a group of delta in an array
 * 4. Find out the moving average (delta)
 * 5. Determines the charging state based on the defined threshold
 *    --> delta is a dynamic parameter which indicates the battery is in charging stage
 * */

typedef struct{
	int32_t batteryVoltage[VOLTAGE_SAMPLES];
	int32_t deltaMovingAverage;
	bool isCharging;
}batteryMgnt_t;

typedef enum
{
	DISCHARGING = 0,
	CHARGING    = 1
}BatteryState_t;

extern int32_t checkBattery();

/*The signal determines ETU State
 *It's also sent to the dash-board to notify the charging state
 * */
extern bool isCharging();

void battery_StateInit(BatteryState_t *batteryHandle);

void battery_NextState(BatteryState_t *batteryHandle,BatteryState_t batterState);

BatteryState_t battery_GetState(BatteryState_t *batteryHandle);


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_BATTERY_MANAGER_H_ */
