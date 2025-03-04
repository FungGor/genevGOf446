/*
 * CMFAX103_thermal_sensor.c
 *
 *  CMFAX103F3950FB - Thermal sensor
 *  Temperature – Resistance Interpolation Curves
 *
 *  Created on: 23 Jan 2025
 *      Author: Chee
 */

#include "CMFAX103F3950FB.h"

/** @defgroup Motor NTC Temperature Sensor
  * @brief Allows to read the temperature of the BLDC Hub Motor
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



/* Functions ---------------------------------------------------- */

/**
 * @brief Initializes motor temperature sensing conversions
 *
 *  @p pHandle : Pointer on Handle structure of Motor TemperatureSensor component
 *
 *  @p pPWMnCurrentSensor : Handle on the PWMC component to be used for regular conversions
 */
__weak void MOTORTEMP_Init(MotorTemp_Handle_t *pHandle)
{
	/* Need to be register with RegularConvManager */
	pHandle->convHandle = RCM_RegisterRegConv(&pHandle->motorTemp);
	MOTORTEMP_Clear(pHandle);
}


/**
 * @brief Initializes internal average temperature computed value
 *
 *  @p pHandle : Pointer on Handle structure of TemperatureSensor component
 */
__weak void MOTORTEMP_Clear(MotorTemp_Handle_t *pHandle)
{
	pHandle->avgMotorTemp = 0u;
	pHandle->avgNTCResistance = 0u;
	pHandle->temperatureBuffer[pHandle->lowPassFilterBandwidth] = (uint16_t){0};

	pHandle->_Super.AvTemp = 0u;
	pHandle->_Super.AvTemp_s16R = 0u;
	pHandle->_Super.AvResistance_R = 0u;
	pHandle->_Super.AvResistance_s16R = 0u;
	pHandle->_Super.LatestConv = 0u;
	pHandle->_Super.FaultState = 0u;
}

/**
  * @brief Performs the Motor NTC average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of MotorTemp_Handle_t component
  *
  *  @r Fault status : Error reported in case of an over temperature detection (if necessary)
  */
__weak uint16_t MOTORTEMP_CalcAvTemperatureOrigin(MotorTemp_Handle_t *pHandle)
{
	uint32_t wTemp; /*Final result of raw ADC samples*/
	uint16_t hAux; /*Raw ADC Values of NTC CMFAX103F3950FB*/
	uint8_t i;
	/*Performs ADC Conversion to get the raw data*/
    hAux = RCM_ExecRegularConv(pHandle->convHandle);
    if( hAux != 0xFFFFu)
    {
    	/*Put all raw ADC values (Samples) of NTC CMFAX103F3950FB into currentBuffer samples */
    	pHandle->temperatureBuffer[pHandle->index] = hAux;
    	wTemp = 0;
    	for(i = 0; i < pHandle->lowPassFilterBandwidth; i++)
    	{
    		/*Sum the samples of raw ADC data*/
    		wTemp += pHandle->temperatureBuffer[i];
    	}
    	/*Final Result*/
    	wTemp = wTemp / pHandle->lowPassFilterBandwidth;
    	pHandle->_Super.AvTemp_s16R = ( uint16_t ) wTemp;
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
    return (pHandle->_Super.AvTemp_s16R) ;
}

/**
  * @brief Performs the temperature sensing average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of TemperatureSensor component
  *
  *  @r Fault status : Error reported in case of an over temperature detection
  */

extern int motorTempOffset50C(uint32_t R_value) {
    float temp_value;
    uint8_t region;
    if (R_value <= 341){
        region = 1;
        temp_value = 126;
    }
    else if ((R_value <= 1249) && (R_value > 341)) {
        region = 2;
        temp_value = 925.83 * pow(R_value,(-0.342));
    }
    else if ((R_value <= 5313) && (R_value > 1249)) {
        region = 3;
        temp_value = -27.63 * log(R_value) + 276.32;
    }
    else if ((R_value <= 34548) && (R_value > 5313)) {
        region = 4;
        temp_value = -21.97 * log(R_value) + 227.71;
    }
    else if ((R_value <= 345275) && (R_value > 34548)) {
        region = 5;
        temp_value = -17.01 * log(R_value) + 175.95;
    }
    else { // R_value > 345275
        region = 6;
        temp_value = -41;
    }

    float temp_value_Offset50_Celcius = temp_value + 50;
    return (round(temp_value_Offset50_Celcius));

}


