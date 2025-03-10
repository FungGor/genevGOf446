/*
 * driver_temp.h
 *
 *  Created on: Mar 4, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MOTOR_SENSORS_MOTOR_DRIVER_NTC_DRIVER_TEMP_H_
#define APPLICATION_MOTOR_SENSORS_MOTOR_DRIVER_NTC_DRIVER_TEMP_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stdint.h"
#include "mc_type.h"

typedef struct
{
	uint16_t conversionParam;           /*!<Convert Temperature from s16R to celsius*/
	uint16_t LatestConv;                /*!< It contains latest Temperature Voltage converted value
                                             expressed in s16Voltage format */
	uint16_t AvDriverOutputVoltage;     /*!<Average output NTC Voltage which is obtained by conversion factors*/
	uint16_t AvDriverTemp;              /*!<Temperatrue in Celsius which is obtained by conversion factors*/
	uint16_t AvDriverTemp_s16R;         /*!<Temperature in s16R format which is obtained by Moving Average*/
	uint32_t AvDriverResistance_R;      /*!<NTC Resistance in R format which is obtained by Moving Average*/
	uint16_t FaultState;                /*!<Reports Fault or OverTemperature (?)*/
}DriverNTC_Handle_t;

uint16_t DRIVER_GetAvgNTCOutputVoltage_d(DriverNTC_Handle_t *pHandle);
uint32_t DRIVER_GetAvgNTCResistance_R(DriverNTC_Handle_t *pHandle);
uint16_t DRIVER_GetTemperature_d(DriverNTC_Handle_t *pHandle);
uint16_t DRIVER_GetAvgTemperature_d(DriverNTC_Handle_t *pHandle);
uint16_t DRIVER_GetAvgTemperature_C(DriverNTC_Handle_t *pHandle);
uint16_t DRIVER_CheckTemperature(DriverNTC_Handle_t *pHandle);


#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif /* APPLICATION_MOTOR_SENSORS_MOTOR_DRIVER_NTC_DRIVER_TEMP_H_ */
