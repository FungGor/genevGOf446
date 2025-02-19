/*
 * battery_current_sensors.h
 *
 *  Created on: Jan 27, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MOTOR_SENSORS_BATTERY_BATTERY_CURRENT_SENSORS_H_
#define APPLICATION_MOTOR_SENSORS_BATTERY_BATTERY_CURRENT_SENSORS_H_
#ifdef _cplusplus
extern "C"
{
#endif
#include "regular_conversion_manager.h"
#include "bus_current_sensor.h"

typedef struct
{
	BusCurrentSensor_Handle_t _Super;

	RegConv_t batteryCurrentRegConv;
	uint16_t rawCurrent;              /*!< Raw Data of Current from ADC ranging from 0 to 65535*/
	uint16_t avBatteryCurrent_s16A;   /*!< Moving Average of BatteryCurrent ranging from 0 to 65535 .*/
	uint16_t sum_current_s16A;        /*!< The sum of sampled BatteryCurrent ranging from 0 to 65535 .*/
	uint16_t old_sample_current_s16A; /*!< Temporary Samples of BatteryCurrent*/
	uint16_t LowPassFilterBW;         /*!< Total Number of Samples that you want (?).*/
	uint16_t *currentBuffer;          /*!< Buffer used to compute average value.*/
	uint16_t elem;                    /*!< Number of stored elements in the average buffer.*/
	uint8_t index;                    /*!< Index of last stored element in the average buffer.*/
	uint8_t convHandle;               /*!< handle to the regular conversion */
}BatteryCurrent_Handle_t;

/* Initialize Battery Current Sensor parameters */
void BATTERYCURRENT_Init(BatteryCurrent_Handle_t *pHandle);

/* Clear static average battery current value */
void BATTERYCURRENT_Clear(BatteryCurrent_Handle_t *pHandle);

/* Battery Current sensing computation s16A with ADC value (Origin Moving Average Algorithm)*/
uint16_t BATTERYCURRENT_CalcAvCurrentOrigin(BatteryCurrent_Handle_t *pHandle);

/* Battery Current sensing computation s16A with ADC value (Advanced Moving Average Algorithm)*/
uint16_t BATTERYCURRENT_CalcCurrentMovAvg(BatteryCurrent_Handle_t *pHandle);

/* Get averaged Battery Current expressed in milli-Ampere */
int16_t BATTERYCURRENT_CalcAvCurrent_a(BatteryCurrent_Handle_t *pHandle);

/* Get the Battery Current fault status */
uint16_t BATTERYCURRENT_CheckBatteryStatus(BatteryCurrent_Handle_t *pHandle);

void BATTERYCURRENT_SetRawCurrent(uint16_t current);

uint16_t BATTERYCURRENT_getRawCurrent();
#ifdef _cplusplus
}
#endif
#endif /* APPLICATION_MOTOR_SENSORS_BATTERY_BATTERY_CURRENT_SENSORS_H_ */
