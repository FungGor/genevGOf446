/*
 * bus_current_sensor.h
 *
 *  Created on: Feb 14, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MOTOR_SENSORS_BATTERY_BUS_CURRENT_SENSOR_H_
#define APPLICATION_MOTOR_SENSORS_BATTERY_BUS_CURRENT_SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mc_type.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup BusCurrentSensor
  * @{
  */

/**
  * @brief  BusCurrentSensor handle definition
  */
typedef struct
{
	uint16_t ConversionFactor;    /*!<Convert Bus Current from s16A to milli-Amps*/
	uint16_t LatestConv;          /*!< It contains latest VCurrent converted value
                                   expressed in u16Current format */
	uint16_t AvBusCurrent_mA;     /*!<Battery current in Milli-Ampere which is obtained by conversion factors*/
	uint16_t AvBusCurrent_s16A;   /*!<Battery current in s16A format which is obtained by Moving Average*/
	uint16_t FaultState;          /*!<Reports Fault or OverCurrent (?)*/

}BusCurrentSensor_Handle_t;

/* Exported functions ------------------------------------------------------- */
uint16_t CBS_GetBusCurrent_d(BusCurrentSensor_Handle_t *pHandle);
uint16_t CBS_GetAvgBusCurrent_d(BusCurrentSensor_Handle_t *pHandle);
uint16_t CBS_GetAvgBusCurrent_A(BusCurrentSensor_Handle_t *pHandle);
uint16_t CBS_CheckBattery(BusCurrentSensor_Handle_t *pHandle);

#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif /* APPLICATION_MOTOR_SENSORS_BATTERY_BUS_CURRENT_SENSOR_H_ */
