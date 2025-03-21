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
#include "motor_ntc_param.h"
#include "motor_param.h"

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
	uint16_t avgNTCVoltage;             /**< It contains latest available average NTC Output Voltage.
                                             This parameter is expressed in s16V */
	int32_t avgMotorTemp;              /**< It contains latest available average Motor Temperature.
                                             This parameter is expressed in u16Celsius */
	uint32_t avgNTCResistance;          /**< It contains latest available average NTC Resistance.
                                             This parameter is expressed in u16Celsius = ((5*Req)/(Vout*Alpha))-Req */
	uint16_t lowPassFilterBandwidth;
	uint16_t overTemperatureThreshold;  /**< Represents the over voltage protection intervention threshold.
                                           This parameter is expressed in u16Celsius through formula:
                                           hOverTempThreshold =
                                           (V0[V]+dV/dT[V/°C]*(OverTempThreshold[°C] - T0[°C]))* 65536 / MCU supply voltage */

	float  alpha;                       /*!< alpha = (RESISTANCE_OUTPUT + RESISTANCE_INPUT)/RESISTANCE_OUTPUT */

    float Resistance_Equivalent;        /*!< Req = (RESISTANCE_OUTPUT + RESISTANCE_INPUT) || RESISTANCE_MOTOR */

    uint16_t *temperatureVoltageBuffer; /*!< Buffer used to compute average value.*/
    uint16_t elem;                      /*!< Number of stored elements in the average buffer.*/
    uint8_t index;                      /*!< Index of last stored element in the average buffer.*/
	uint8_t  convHandle;                /**!< handle to the regular conversion */
}MotorTemp_Handle_t;

/* Initialize Motor NTC Sensor parameters */
extern void MOTORTEMP_Init(MotorTemp_Handle_t *pHandle);

/* Clear static average Motor NTC value */
extern void MOTORTEMP_Clear(MotorTemp_Handle_t *pHandle);

/* Motor NTC Sensor output voltage computation s16A with ADC value (Origin Moving Average Algorithm)*/
extern void MOTORTEMP_CalcAvOutputVoltageOrigin(MotorTemp_Handle_t *pHandle);

/* Motor NTC Sensor output resistance computation based on s16A ADC value from MOTORTEMP_CalcAvOutputVoltageOrigin */
extern void MOTORTEMP_CalcAvR_Value(MotorTemp_Handle_t *pHandle);

/* Motor NTC Sensor output temperature computation based on s16A ADC value from MOTORTEMP_CalcAvTemp_Value */
extern void MOTORTEMP_CalcAvTemp_Value(MotorTemp_Handle_t *pHandle);

#ifdef _cplusplus
}
#endif


#endif /* HARDWARE_CMFAX103_THERMAL_SENSOR_H_ */
