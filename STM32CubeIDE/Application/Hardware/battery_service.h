/*
 * battery_manager.h
 *
 *  Created on: Jul 18, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_BATTERY_SERVICE_H_
#define APPLICATION_HARDWARE_BATTERY_SERVICE_H_

#ifdef __cplusplus
extern "C"{
#endif
#include "motor_param.h"

#define  WINDOW_SIZE                   20
#define  MOVING_AVERAGE_COEFFICIENT     5
#define  CHARGING_RATE_THRESHOLD       50
#define  DISCHARGING_RATE_THRESHOLD    50

/*                    Battery Service
 *  a) Monitors Battery Charging / Discharging Status
 *  b) Calculates +dv/dt or -dv/dt while charging / discharging
 * To find out charging rate............
 * 1. Regularly checks the battery voltage with 10 raw samples
 * 2. Calculates delta while checking the battery voltage
 * 3. Store a group of delta in an array
 * 4. Find out the moving average (delta)
 * 5. Determines the charging state based on the defined threshold
 *    --> delta is a dynamic parameter which indicates the battery is in charging stage
 *
 * when r = dv/dt closes to zero, system should exit the Charging State.....
 * (Optional) You could make use of int32_t deltaMovingAverage to find out the battery's discharging rate
 * Provides a feature for battery health monitoring
 *
 * For GenevGO's application, this module extension is not necessary, but it might be useful in the future's development.
 * */

typedef enum
{
	DISCHARGING = 0,
	CHARGING    = 1
}BatteryState_t;

typedef struct{
	BatteryState_t batteryState;
	int32_t batteryVoltage[WINDOW_SIZE];
	int32_t instant_delta_MovingAverage;          /*r = +- dv/dt which determines if the battery is chargine / discharging*/
	int32_t delta_ChargingRateMovingAverage;      /*r = +dv/dt  units: + mV/s */
	int32_t delta_DischargingRateMovingAverage;   /*r = -dv/dt  units: - mV/s */
	bool isCharging;
}batteryMgnt_t;

/*Initialize Battery Service*/
extern void batteryServiceInit(batteryMgnt_t *batteryMgnt);

/*Should be called regularly*/
extern int32_t checkBattery();

/*The rate of charging or discharging += dv/dt*/
extern void calRate();

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
#endif /* APPLICATION_HARDWARE_BATTERY_SERVICE_H_ */
