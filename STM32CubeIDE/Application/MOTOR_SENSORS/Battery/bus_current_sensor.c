/*
 * bus_current_sensor.c
 *
 *  Created on: Feb 14, 2025
 *      Author: TerenceLeung
 */
#include "bus_current_sensor.h"

/**
  * @brief  It return latest Cbus conversion result (raw Conversion) expressed in u16Amp
  * @param  pHandle related Handle of BusCurrentSensor_Handle_t
  * @retval uint16_t Latest Cbus conversion result in digit
  */
__weak uint16_t CBS_GetBusCurrent_d(BusCurrentSensor_Handle_t *pHandle)
{
	return (pHandle->LatestConv);
}

#if defined (CCMRAM)
#if defined (__ICCARM__)
#pragma location = ".ccmram"
#elif defined (__CC_ARM) || defined(__GNUC__)
__attribute__( ( section ( ".ccmram" ) ) )
#endif
#endif
/**
  * @brief  It return latest averaged Cbus measurement expressed in u16Ampere
  * @param  pHandle related Handle of BusCurrentSensor_Handle_t
  * @retval uint16_t Latest averaged Cbus measurement in digit
  */
__weak uint16_t CBS_GetAvgBusCurrent_d(BusCurrentSensor_Handle_t *pHandle)
{
	return (pHandle->AvBusCurrent_s16A);
}

/**
  * @brief  It return latest averaged Cbus measurement expressed in Amps
  * @param  pHandle related Handle of BusCurrentSensor_Handle_t
  * @retval uint16_t Latest averaged Cbus measurement in Volts
  */
__weak uint16_t CBS_GetAvgBusCurrent_A(BusCurrentSensor_Handle_t *pHandle)
{
	uint32_t temp;

	temp = (uint32_t)(pHandle->AvBusCurrent_s16A);
	temp *= pHandle->ConversionFactor;
	temp /= 65536u;

	pHandle->AvBusCurrent_mA = (uint16_t)temp;

	return (pHandle->AvBusCurrent_mA);
}

/**
  * @brief  It returns NORMAL, OVERCURRENT depending on
  *         bus current and protection threshold values
  * @param  pHandle related Handle of BusCurrentSensor_Handle_t
  * @retval uint16_t Fault code error
  */
__weak uint16_t CBS_CheckBattery( BusCurrentSensor_Handle_t * pHandle )
{
  return ( pHandle->FaultState );
}
