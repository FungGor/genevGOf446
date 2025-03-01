/*
 * motor_ntc.h
 *
 *  Created on: Feb 28, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_H_
#define APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "mc_type.h"
/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup Motor NTC
  * @{
  */

/**
  * @brief  Motor NTC handle definition
  */
typedef struct
{
	uint16_t conversionParam;     /*!<Convert Temperature from s16R to celsius*/
	uint16_t LatestConv;          /*!< It contains latest Temperature converted value
                                   expressed in u16Temperature format */
	uint16_t AvTemp;              /*!<Temperatrue in Celsius which is obtained by conversion factors*/
	uint16_t AvTemp_s16R;         /*!<Temperature in s16R format which is obtained by Moving Average*/
	uint16_t AvResistance_s16R;   /*!<NTC Resistance in s16R format which is obtained by Moving Average*/
	uint16_t AvResistance_R;      /*!<NTC Resistance in R format which is obtained by Moving Average*/
	uint16_t FaultState;          /*!<Reports Fault or OverTemperature (?)*/
}MotorNTCSensor_Handle_t;

uint16_t MOTOR_GetAvgNTCResistance_d(MotorNTCSensor_Handle_t *pHandle);
uint16_t MOTOR_GetAvgNTCResistance_R(MotorNTCSensor_Handle_t *pHandle);
uint16_t MOTOR_GetTemperature_d(MotorNTCSensor_Handle_t *pHandle);
uint16_t MOTOR_GetAvgTemperature_d(MotorNTCSensor_Handle_t *pHandle);
uint16_t MOTOR_GetAvgTemperature_C(MotorNTCSensor_Handle_t *pHandle);
uint16_t MOTOR_CheckTemperature(MotorNTCSensor_Handle_t *pHandle);

#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif /* APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_H_ */
