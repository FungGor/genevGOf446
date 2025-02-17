/*
 * battery_current_sensors.c
 *
 *  Created on: Jan 27, 2025
 *      Author: TerenceLeung
 */
#include "battery_current_sensors.h"
/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup BatteryCurrentSensor
  * @{
  */

/** @defgroup BatteryCurrentSensor BatteryCurrentSensor Sensor
  * @brief BatteryCurrentSensor implementation
  *
  * @todo Document the BatteryCurrentSensor "module".
  *
  * @{
  */

/**
  * @brief  It initializes Battery Current Sensor Conversion(ADC, ADC channel, conversion time.
    It must be called only after PWMC_Init.
  * @param  pHandle related BatteryCurrent_Handle_t
  * @retval none
  */
__weak void BATTERYCURRENT_Init(BatteryCurrent_Handle_t *pHandle)
{
	/* Need to be register with RegularConvManager */
	pHandle->convHandle = RCM_RegisterRegConv(&pHandle->batteryCurrentRegConv);
}

/**
 * @brief Initializes internal average battery current computed value s16A ADC Values
 *
 *  @p pHandle : Pointer on Handle structure of BatteryCurrentSensor component
 */
__weak void BATTERYCURRENT_Clear(BatteryCurrent_Handle_t *pHandle)
{
	pHandle->avBatteryCurrent = 0u;
}

/**
  * @brief Performs the battery current average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of BatteryCurrentSensor component
  *
  *  @r Fault status : Error reported in case of an over current detection (if necessary)
  */
__weak uint16_t BATTERYCURRENT_CalcAvCurrent(BatteryCurrent_Handle_t *pHandle)
{
	uint32_t wTemp; /*Final result of raw ADC samples*/
	uint16_t hAux; /*Raw ADC Values of Current Sensor ZXCT1084E5TA*/
	uint8_t i;

	/*Performs ADC Conversion to get the raw data*/
    hAux = RCM_ExecRegularConv(pHandle->convHandle);
    if( hAux != 0xFFFFu)
    {
    	/*Put all raw ADC values (Samples) of ZXCT1084E5TA into currentBuffer samples */
    	pHandle->currentBuffer[pHandle->index] = hAux;
    	wTemp = 0;
    	for(i = 0; i < pHandle->LowPassFilterBW; i++)
    	{
    		/*Sum the samples of raw ADC data*/
    		wTemp += pHandle->currentBuffer[i];
    	}
    	/*Final Result*/
    	wTemp /= pHandle->LowPassFilterBW;
    	pHandle->_Super.AvBusCurrent_d = ( uint16_t ) wTemp;
    	pHandle->_Super.LatestConv = hAux;

    	if(pHandle->index < pHandle->LowPassFilterBW - 1)
    	{
    		pHandle->index ++;
    	}
    	else
    	{
    		pHandle->index = 0;
    	}
    }
    return hAux;
}
