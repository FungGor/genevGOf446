/*
 * NTCG163JF103FT.h
 *
 *  NTCG163JF103FT Thermal Sensor
 *
 *  Created on: 24 Jan 2025
 *      Author: Chee
 */

#ifndef HARDWARE_NTCG163JF103FT_H_
#define HARDWARE_NTCG163JF103FT_H_

#ifdef _cplusplus
extern "C"
{
#endif


/*********************************************************************
 * INCLUDES
 */

/* Library Header files */
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "regular_conversion_manager.h"
#include "driver_temp.h"

/*********************************************************************************************
 *  Constants
 *********************************************************************************************/
#define TEMP0               298.15
#define RESISTANCE0         10000
#define BCONSTANT_AV        3425.5
#define UPPER_RESISTANCE    251200  // correspond to -40 degree C
#define LOWER_RESISTANCE    500     // correspond to 130 degree C
#define ABSOLUTE_TEMP       273.15

/*********************************************************************
 * TYPEDEFS
 */


/*********************************************************************
 * MACROS
 */



/*********************************************************************
 * API FUNCTIONS
 */

/*********************************************************************
 * FUNCTIONS
 *********************************************************************/

typedef struct
{
	DriverNTC_Handle_t _Super;
	RegConv_t driverTemp;
	uint16_t avgDriverNTCVoltage;             /**< It contains latest available average NTC Output Voltage.
                                                This parameter is expressed in s16V */
	uint16_t avgDriverTemp;                   /**< It contains latest available average Motor Temperature.
	                                             This parameter is expressed in u16Celsius */
	uint32_t avgDriverNTCResistance;          /**< It contains latest available average NTC Resistance.
	                                             This parameter is expressed in u16Celsius */
	uint16_t lowPassFilterBandwidth;
	uint16_t overTemperatureThreshold;        /**< Represents the over voltage protection intervention threshold.
                                                 This parameter is expressed in u16Celsius through formula:
                                                 hOverTempThreshold =
                                                 (V0[V]+dV/dT[V/°C]*(OverTempThreshold[°C] - T0[°C]))* 65536 / MCU supply voltage */
    int32_t  conversionFactor;
    uint16_t *driverNTCVoltageBuffer;         /*!< Buffer used to compute average value.*/
    uint16_t elem;                            /*!< Number of stored elements in the average buffer.*/
    uint8_t index;                            /*!< Index of last stored element in the average buffer.*/

    int16_t  sensitivity;                     /**< NTC sensitivity
                                                This parameter is equal to MCU supply voltage [V] / dV/dT [V/°C] */
    uint32_t V0;                              /**< V0 voltage constant value used to convert the temperature into Volts.
                                                 This parameter is equal V0*65536/MCU supply
                                                 Used in through formula: V[V]=V0+dV/dT[V/°C]*(T-T0)[°C] */
    uint16_t T0;                              /**< T0 temperature constant value used to convert the temperature into Volts
                                                 Used in through formula: V[V]=V0+dV/dT[V/°C]*(T-T0)[°C] */
	uint8_t  convHandle;                      /**!< handle to the regular conversion */
}DriverTemp_Handle_t;

/* Initialize Motor Driver NTC Sensor parameters */
void MOTORDRIVERTEMP_Init(DriverTemp_Handle_t *pHandle);

/* Clear static average Motor Driver NTC value */
void MOTORDRIVERTEMP_Clear(DriverTemp_Handle_t *pHandle);

/* Motor NTC Sensor output voltage computation s16A with ADC value (Origin Moving Average Algorithm)*/
uint16_t MOTORDRIVERTEMP_CalcAvOutputVoltageOrigin(DriverTemp_Handle_t *pHandle);

/* Motor NTC Sensor output resistance computation based on s16A ADC value from MOTORDRIVERTEMP_CalcAvOutputVoltageOrigin */
uint32_t MOTORDRIVERTEMP_CalcAvR_Value(DriverTemp_Handle_t *pHandle);

extern int heatSinkTempOffset50C(uint32_t R_value);


#ifdef _cplusplus
}
#endif






#endif /* HARDWARE_NTCG163JF103FT_H_ */
