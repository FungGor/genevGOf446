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
#include "motor_param.h"

/*********************************************************************************************
 *  Constants
 *********************************************************************************************/
#define TEMP0                                   (double)298.15
#define RESISTANCE0                             (float)10000
#define RESISTANCE_DIVIDER                      (float)5600
#define BCONSTANT_AV                            (double)3425.5
#define UPPER_RESISTANCE                        251200  // correspond to -40 degree C
#define LOWER_RESISTANCE                        500     // correspond to 130 degree C
#define ABSOLUTE_TEMP                           (double)273.15
#define MOTOR_DRIVER_TEMP_BANDWIDTH_FACTOR      200
#define MAXIMUM_ADC_RANGE                       65536
#define NTC_VDD                                 3.3
#define MOTOR_DRIVER_TEMP_SAMPLING              LL_ADC_SAMPLING_CYCLE(3)

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
	int32_t avgDriverTemp;                   /**< It contains latest available average Motor Temperature.
	                                             This parameter is expressed in u16Celsius */
	uint32_t avgDriverNTCResistance;          /**< It contains latest available average NTC Resistance.
	                                             This parameter is expressed in u16Celsius */
	uint16_t lowPassFilterBandwidth;
	uint16_t overTemperatureThreshold;        /**< Represents the over voltage protection intervention threshold.
                                                 This parameter is expressed in u16Celsius through formula:
                                                 hOverTempThreshold =
                                                 (V0[V]+dV/dT[V/°C]*(OverTempThreshold[°C] - T0[°C]))* 65536 / MCU supply voltage */
    uint16_t *driverNTCVoltageBuffer;         /*!< Buffer used to compute average value.*/
    uint16_t elem;                            /*!< Number of stored elements in the average buffer.*/
    uint8_t index;                            /*!< Index of last stored element in the average buffer.*/
	uint8_t  convHandle;                      /**!< handle to the regular conversion */
}DriverTemp_Handle_t;

/* Initialize Motor Driver NTC Sensor parameters */
extern void MOTORDRIVERTEMP_Init(DriverTemp_Handle_t *pHandle);

/* Clear static average Motor Driver NTC value */
extern void MOTORDRIVERTEMP_Clear(DriverTemp_Handle_t *pHandle);

/* Motor NTC Sensor output voltage computation s16A with ADC value (Origin Moving Average Algorithm)*/
extern void MOTORDRIVERTEMP_CalcAvOutputVoltageOrigin(DriverTemp_Handle_t *pHandle);

/* Motor NTC Sensor output resistance computation based on s16A ADC value from MOTORDRIVERTEMP_CalcAvOutputVoltageOrigin */
extern void MOTORDRIVERTEMP_CalcAvR_Value(DriverTemp_Handle_t *pHandle);

/* Motor NTC Sensor output temperature computation based on s16A ADC value from MOTORTEMP_CalcAvTemp_Value */
extern void heatSinkTempOffset50C(DriverTemp_Handle_t *pHandle);


#ifdef _cplusplus
}
#endif






#endif /* HARDWARE_NTCG163JF103FT_H_ */
