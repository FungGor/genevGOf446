/*
 * CMFAX103_thermal_sensor.h
 *
 *  Created on: 23 Jan 2025
 *      Author: Chee
 */

#ifndef HARDWARE_CMFAX103_THERMAL_SENSOR_H_
#define HARDWARE_CMFAX103_THERMAL_SENSOR_H_

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
#include "motor_ntc.h"

/*********************************************************************************************
 *  Constants
 *********************************************************************************************/

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
	MotorNTCSensor_Handle_t  _Super;
	RegConv_t motorTemp;
	uint16_t avgMotorTemp;              /**< It contains latest available average Motor Temperature.
                                             This parameter is expressed in u16Celsius */
	uint16_t avgNTCResistance;          /**< It contains latest available average NTC Resistance.
                                             This parameter is expressed in u16Celsius */
	uint16_t lowPassFilterBandwidth;
	uint16_t overTemperatureThreshold;  /**< Represents the over voltage protection intervention threshold.
                                           This parameter is expressed in u16Celsius through formula:
                                           hOverTempThreshold =
                                           (V0[V]+dV/dT[V/°C]*(OverTempThreshold[°C] - T0[°C]))* 65536 / MCU supply voltage */
    int32_t  conversionFactor;

    uint16_t *temperatureBuffer;        /*!< Buffer used to compute average value.*/
    uint16_t elem;                      /*!< Number of stored elements in the average buffer.*/
    uint8_t index;                      /*!< Index of last stored element in the average buffer.*/

    int16_t  sensitivity;              /**< NTC sensitivity
                                            This parameter is equal to MCU supply voltage [V] / dV/dT [V/°C] */
    uint32_t V0;                       /**< V0 voltage constant value used to convert the temperature into Volts.
                                            This parameter is equal V0*65536/MCU supply
                                            Used in through formula: V[V]=V0+dV/dT[V/°C]*(T-T0)[°C] */
    uint16_t T0;                       /**< T0 temperature constant value used to convert the temperature into Volts
                                            Used in through formula: V[V]=V0+dV/dT[V/°C]*(T-T0)[°C] */
	uint8_t  convHandle;               /**!< handle to the regular conversion */
}MotorTemp_Handle_t;

/* Initialize Motor NTC Sensor parameters */
void MOTORTEMP_Init(MotorTemp_Handle_t *pHandle);

/* Clear static average Motor NTC value */
void MOTORTEMP_Clear(MotorTemp_Handle_t *pHandle);

/* Motor NTC Sensor Temperature computation s16A with ADC value (Origin Moving Average Algorithm)*/
uint16_t MOTORTEMP_CalcAvTemperatureOrigin(MotorTemp_Handle_t *pHandle);

uint32_t MOTORTEMP_CalcAvR_Value(MotorTemp_Handle_t *pHandle);

extern int motorTempOffset50C(uint32_t R_value);


#ifdef _cplusplus
}
#endif


#endif /* HARDWARE_CMFAX103_THERMAL_SENSOR_H_ */
