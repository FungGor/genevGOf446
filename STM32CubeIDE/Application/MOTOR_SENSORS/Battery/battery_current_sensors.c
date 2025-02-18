/*
 * battery_current_sensors.c
 *
 *  Created on: Jan 27, 2025
 *      Author: TerenceLeung
 */
#include "battery_current_sensors.h"
#include "current_sensor_parameter.h"
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
	BATTERYCURRENT_Clear(pHandle);
}

/**
 * @brief Initializes internal average battery current computed value s16A ADC Values (Parameter Initialization)
 *
 *  @p pHandle : Pointer on Handle structure of BatteryCurrentSensor component
 */
__weak void BATTERYCURRENT_Clear(BatteryCurrent_Handle_t *pHandle)
{
	pHandle->avBatteryCurrent_s16A = 0u;
	pHandle->rawCurrent = 0u;
	pHandle->sum_current_s16A = 0u;
	pHandle->old_sample_current_s16A = 0u;
	pHandle->currentBuffer[pHandle->LowPassFilterBW] = (uint16_t){0};
	pHandle->index = 0u;
	pHandle->elem = 0u;

	pHandle->_Super.AvBusCurrent_mA = 0u;
	pHandle->_Super.LatestConv = 0u;
	pHandle->_Super.FaultState = 0;

}

/**
  * @brief Performs the battery current average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of BatteryCurrentSensor component
  *
  *  @r Fault status : Error reported in case of an over current detection (if necessary)
  */
__weak uint16_t BATTERYCURRENT_CalcAvCurrentOrigin(BatteryCurrent_Handle_t *pHandle)
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
    	pHandle->_Super.AvBusCurrent_s16A = ( uint16_t ) wTemp;
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

/**
  * @brief Performs the battery current Moving Average computation after an ADC conversion
  *
  *  @p pHandle : Pointer on Handle structure of BatteryCurrentSensor component
  *
  *  @r Fault status : Error reported in case of an over current detection (if necessary)
  */
__weak uint16_t BATTERYCURRENT_CalcCurrentMovAvg(BatteryCurrent_Handle_t *pHandle)
{
	pHandle->old_sample_current_s16A  = pHandle->currentBuffer[pHandle->elem];
	pHandle->currentBuffer[pHandle->elem] = RCM_ExecRegularConv(pHandle->convHandle);
	/*Stores the latest ADC Conversion Values*/
	pHandle->_Super.LatestConv = pHandle->currentBuffer[pHandle->elem];
	pHandle->sum_current_s16A = pHandle->sum_current_s16A - pHandle->old_sample_current_s16A + pHandle->currentBuffer[pHandle->elem];
	if(pHandle->index < pHandle->LowPassFilterBW)
	{
		pHandle->index++;
	}
    // Initially, the array is filled by ZEROS that are not measurements.  We don't take non-measurements into account.
	// Once the array is filled with actual measurements, kk remains constant and == ARRAYSIZE.
	pHandle->avBatteryCurrent_s16A = pHandle->sum_current_s16A / pHandle->index;
	pHandle->elem++;
	if(pHandle->elem >= pHandle->LowPassFilterBW)
	{
		pHandle->elem = 0;
		pHandle->_Super.AvBusCurrent_s16A = pHandle->avBatteryCurrent_s16A;
		return pHandle->avBatteryCurrent_s16A;
	}
	return (pHandle->avBatteryCurrent_s16A);
}

/**
  * @brief  It returns ERROR OR NO_ERROR depending on
  *         bus current and protection threshold values
  * @param  pHandle related RDivider_Handle_t
  * @retval uint16_t Fault code error
  */
__weak uint16_t BATTERYCURRENT_CheckBatteryStatus(BatteryCurrent_Handle_t *pHandle)
{
	uint16_t status = 0x01;
	return status;

}
