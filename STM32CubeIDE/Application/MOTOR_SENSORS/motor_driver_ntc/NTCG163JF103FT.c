/*
 * NTCG163JF103FT.c
 *
 *  Created on: 24 Jan 2025
 *      Author: Chee
 */

#include "NTCG163JF103FT.h"

/**
 * @brief Initializes motor driver temperature sensing conversions
 *
 *  @p pHandle : Pointer on Handle structure of Motor DriverTemp_Handle_t component
 *
 *  @p pPWMnCurrentSensor : Handle on the PWMC component to be used for regular conversions
 */
__weak void MOTORDRIVERTEMP_Init(DriverTemp_Handle_t *pHandle)
{

}

/**
 * @brief Initializes internal average temperature computed value
 *
 *  @p pHandle : Pointer on Handle structure of DriverTemp_Handle_t component
 */
__weak void MOTORDRIVERTEMP_Clear(DriverTemp_Handle_t *pHandle)
{

}

/**
  * @brief Performs the Motor Driver NTC average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of DriverTemp_Handle_t component
  *
  *  @r Fault status : Error reported in case of an over temperature detection (if necessary)
  */
__weak uint16_t MOTORDRIVERTEMP_CalcAvOutputVoltageOrigin(DriverTemp_Handle_t *pHandle)
{

}

extern int heatSinkTempOffset50C(uint32_t R_value)
{
    if (R_value > UPPER_RESISTANCE) {
        R_value = UPPER_RESISTANCE;
    }
    if (R_value < LOWER_RESISTANCE) {
        R_value = LOWER_RESISTANCE;
    }

    double Temp = TEMP0;    // absolute temperature Kelvin
    double temp_offSet50C = 75;

    /*  NTCG163JF103FT Thermistor temperature as a function of resistance  */
    Temp = (BCONSTANT_AV * TEMP0) / (TEMP0 * log((double) R_value/RESISTANCE0) + BCONSTANT_AV);

    temp_offSet50C = Temp - ABSOLUTE_TEMP + 50; // degree Celcius + 50 degree Celcius

    return (round(temp_offSet50C));

}
