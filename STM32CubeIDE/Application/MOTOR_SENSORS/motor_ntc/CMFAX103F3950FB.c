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
	pHandle->avgNTCVoltage = 0u;
	pHandle->avgMotorTemp = 0u;
	pHandle->avgNTCResistance = 0u;
	pHandle->temperatureVoltageBuffer[pHandle->lowPassFilterBandwidth] = (uint16_t){0};

	pHandle->_Super.AvOutputVoltage = 0u;
	pHandle->_Super.AvTemp = 0u;
	pHandle->_Super.AvTemp_s16R = 0u;
	pHandle->_Super.AvResistance_R = 0u;
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
__weak void MOTORTEMP_CalcAvOutputVoltageOrigin(MotorTemp_Handle_t *pHandle)
{
	uint32_t wTemp; /*Final result of raw ADC samples*/
	uint16_t hAux; /*Raw ADC Values of NTC CMFAX103F3950FB*/
	uint8_t i;
	/*Performs ADC Conversion to get the raw data*/
    hAux = RCM_ExecRegularConv(pHandle->convHandle);
    if( hAux != 0xFFFFu)
    {
    	/*Put all raw ADC values (Samples) of NTC CMFAX103F3950FB into currentBuffer samples */
    	pHandle->temperatureVoltageBuffer[pHandle->index] = hAux;
    	wTemp = 0;
    	for(i = 0; i < pHandle->lowPassFilterBandwidth; i++)
    	{
    		/*Sum the samples of raw ADC data*/
    		wTemp += pHandle->temperatureVoltageBuffer[i];
    	}
    	/*Final Result*/
    	wTemp = wTemp / pHandle->lowPassFilterBandwidth;
    	pHandle->avgNTCVoltage = (uint16_t) wTemp;
    	pHandle->_Super.AvOutputVoltage = ( uint16_t ) wTemp;
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
__weak void MOTORTEMP_CalcAvR_Value(MotorTemp_Handle_t *pHandle)
{
	float outputVoltage = 0;
	float resistanceTemp = 0;
	uint32_t NTC_R = 0;
    outputVoltage = ((float)(pHandle->avgNTCVoltage)/CONVERSION_RANGE)*ADC_VOLTAGE_RANGE;
    resistanceTemp = (NTC_MOTOTR_SUPPLY_VOLTAGE*pHandle->Resistance_Equivalent);
    resistanceTemp = resistanceTemp / (outputVoltage*pHandle->alpha);
    resistanceTemp = resistanceTemp - pHandle->Resistance_Equivalent;
    NTC_R = (uint32_t) resistanceTemp;
    pHandle->avgNTCResistance = NTC_R;
}

/**
  * @brief Performs the temperature sensing average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of TemperatureSensor component
  *
  *  @r Fault status : Error reported in case of an over temperature detection
  *
  *  Get s16A Values from ADC --> Based on the conversion factor, get the output voltage
  *  --> Based on the output voltage, back calculate the resistance with conversion factor
  */
__weak void MOTORTEMP_CalcAvTemp_Value(MotorTemp_Handle_t *pHandle) {
    float temp_value;
    uint8_t region;
    if (pHandle->avgNTCResistance <= 341){
        region = 1;
        temp_value = 126;
    }
    else if ((pHandle->avgNTCResistance <= 1249) && (pHandle->avgNTCResistance > 341)) {
        region = 2;
        temp_value = 925.83 * pow(pHandle->avgNTCResistance,(-0.342));
    }
    else if ((pHandle->avgNTCResistance <= 5313) && (pHandle->avgNTCResistance > 1249)) {
        region = 3;
        temp_value = -27.63 * log(pHandle->avgNTCResistance) + 276.32;
    }
    else if ((pHandle->avgNTCResistance <= 34548) && (pHandle->avgNTCResistance > 5313)) {
        region = 4;
        temp_value = -21.97 * log(pHandle->avgNTCResistance) + 227.71;
    }
    else if ((pHandle->avgNTCResistance <= 345275) && (pHandle->avgNTCResistance > 34548)) {
        region = 5;
        temp_value = -17.01 * log(pHandle->avgNTCResistance) + 175.95;
    }
    else { // R_value > 345275
        region = 6;
        temp_value = -41;
    }
    pHandle->avgMotorTemp = (int32_t) round(temp_value);
    setMotorTemperature(pHandle->avgMotorTemp);
}

__weak bool MOTORTEMP_OVERTEMPERATURE(MotorTemp_Handle_t *pHandle)
{
	if(pHandle->avgMotorTemp > pHandle->overTemperatureThreshold)
	{
		updateMotorTemperatureStatus(true);
		return true;
	}
	return false;
}
