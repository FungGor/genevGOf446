/*
 * NTCG163JF103FT.c
 *
 *  Created on: 24 Jan 2025
 *      Author: Chee
 */

#include "NTCG163JF103FT.h"

/** @defgroup Motor NTC Temperature Sensor
  * @brief Allows to read the temperature of the Motor Driver
  *
  * This component implements both a virtual and a real temperature sensor,
  * depending on the sensor availability.
  *
  * Access to the MCU peripherals needed to acquire the temperature (GPIO and ADC
  * used for regular conversion) is managed by the PWM component used in the Motor
  * Control subsystem. As a consequence, this NTC temperature sensor implementation
  * is hardware-independent.
  *
  * If a real temperature sensor is available (Sensor Type = #REAL_SENSOR),
  * this component can handle NTC sensors or, more generally, analog temperature sensors
  * which output is related to the temperature by the following formula:
  *
  * @f[
  *               V_{out} = V_0 + \frac{dV}{dT} \cdot ( T - T_0)
  * @f]
  *
  * In case a real temperature sensor is not available (Sensor Type = #VIRTUAL_SENSOR),
  * This component will always returns a constant, programmable, temperature.
  *
  * @{
  */

/**
 * @brief Initializes motor driver temperature sensing conversions
 *
 *  @p pHandle : Pointer on Handle structure of Motor DriverTemp_Handle_t component
 *
 *  @p pPWMnCurrentSensor : Handle on the PWMC component to be used for regular conversions
 */
__weak void MOTORDRIVERTEMP_Init(DriverTemp_Handle_t *pHandle)
{
	/* Need to be register with RegularConvManager */
	pHandle->convHandle = RCM_RegisterRegConv(&pHandle->driverTemp);
	MOTORDRIVERTEMP_Clear(pHandle);
}

/**
 * @brief Initializes internal average temperature computed value
 *
 *  @p pHandle : Pointer on Handle structure of DriverTemp_Handle_t component
 */
__weak void MOTORDRIVERTEMP_Clear(DriverTemp_Handle_t *pHandle)
{
	pHandle->avgDriverNTCVoltage = 0u;
	pHandle->avgDriverTemp = 0u;
	pHandle->avgDriverNTCResistance = 0u;
	pHandle->driverNTCVoltageBuffer[pHandle->lowPassFilterBandwidth] = (uint16_t){0};

	pHandle->_Super.AvDriverOutputVoltage = 0u;
	pHandle->_Super.AvDriverTemp = 0u;
	pHandle->_Super.AvDriverTemp_s16R = 0u;
	pHandle->_Super.AvDriverResistance_R = 0u;
	pHandle->_Super.LatestConv = 0u;
	pHandle->_Super.FaultState = 0u;
}

/**
  * @brief Performs the Motor Driver NTC average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of DriverTemp_Handle_t component
  *
  *  @r Fault status : Error reported in case of an over temperature detection (if necessary)
  */
__weak void MOTORDRIVERTEMP_CalcAvOutputVoltageOrigin(DriverTemp_Handle_t *pHandle)
{
	uint32_t wTemp; /*Final result of raw ADC samples*/
	uint16_t hAux; /*Raw ADC Values of NTC CMFAX103F3950FB*/
	uint8_t i;
	/*Performs ADC Conversion to get the raw data*/
    hAux = RCM_ExecRegularConv(pHandle->convHandle);
    if( hAux != 0xFFFFu)
    {
    	/*Put all raw ADC values (Samples) of NTC NTCG163JF103FT into currentBuffer samples */
    	pHandle->driverNTCVoltageBuffer[pHandle->index] = hAux;
    	wTemp = 0;
    	for(i = 0; i < pHandle->lowPassFilterBandwidth; i++)
    	{
    		/*Sum the samples of raw ADC data*/
    		wTemp += pHandle->driverNTCVoltageBuffer[i];
    	}
    	/*Final Result*/
    	wTemp = wTemp / pHandle->lowPassFilterBandwidth;
    	pHandle->avgDriverNTCVoltage = (uint16_t) wTemp;
    	pHandle->_Super.AvDriverOutputVoltage = ( uint16_t ) wTemp;
    	pHandle->_Super.LatestConv = hAux;

    	if(pHandle->index < pHandle->lowPassFilterBandwidth - 1)
    	{
    		pHandle->index ++;
    	}
    	else
    	{
    		pHandle->index = 0;
    	}
    }
}

/**
  * @brief Performs the temperature sensing resistance computation after obtaining s16V Output voltage from ADC Conversion
  *
  *  @p V0 : Average NTC Output Voltage
  *
  *  @r Fault status : Error reported in case of an over temperature detection
  *
  *  Get s16A Values from ADC --> Based on the conversion factor, get the output voltage
  *  --> Based on the output voltage, back calculate the resistance with conversion factor
  */
__weak void MOTORDRIVERTEMP_CalcAvR_Value(DriverTemp_Handle_t *pHandle)
{
	float outputvoltage = 0;
	float resistanceTemp = 0;
	uint32_t NTC_DRIVER = 0;
	outputvoltage = ((float)(pHandle->avgDriverNTCVoltage)/MAXIMUM_ADC_RANGE)*NTC_VDD;
	resistanceTemp = ((NTC_VDD*RESISTANCE_DIVIDER)/outputvoltage)-RESISTANCE_DIVIDER;
	NTC_DRIVER = (uint32_t)resistanceTemp;
	pHandle->avgDriverNTCResistance = NTC_DRIVER;
}

__weak void heatSinkTempOffset50C(DriverTemp_Handle_t *pHandle)
{
    if (pHandle->avgDriverNTCResistance > UPPER_RESISTANCE) {
    	pHandle->avgDriverNTCResistance = UPPER_RESISTANCE;
    }
    if (pHandle->avgDriverNTCResistance < LOWER_RESISTANCE) {
    	pHandle->avgDriverNTCResistance = LOWER_RESISTANCE;
    }

    double Temp = TEMP0;    // absolute temperature Kelvin
    double temp_offSet50C = 75;

    /*  NTCG163JF103FT Thermistor temperature as a function of resistance  */
    Temp = (BCONSTANT_AV * TEMP0) / (TEMP0 * log((double) pHandle->avgDriverNTCResistance/RESISTANCE0) + BCONSTANT_AV);

    temp_offSet50C = Temp - ABSOLUTE_TEMP; // degree Celcius + 50 degree Celcius

    pHandle->avgDriverTemp = (round(temp_offSet50C));
    setDriverTemperature(pHandle->avgDriverTemp);
}

__weak bool MOTORDRIVERTEMP_OVERTEMPERATURE(DriverTemp_Handle_t *pHandle)
{
	if(pHandle->avgDriverTemp > pHandle->overTemperatureThreshold)
	{
		updateDriverTemperatureStatus(true);
		return true;
	}
	return false;
}
