/*
 * driver_temp.c
 *
 *  Created on: Mar 4, 2025
 *      Author: TerenceLeung
 */
#include "driver_temp.h"

/**
  * @brief  It return latest converted MOTOR DRIVER NTC resistance measurement expressed in u32
  * @param  pHandle related Handle of DriverNTC_Handle_t
  * @retval uint32_t Latest averaged MOTOR DRIVER NTC resistance measurement in Resistance
  */
__weak uint32_t DRIVER_GetAvgNTCResistance_R(DriverNTC_Handle_t *pHandle)
{
	return (pHandle->AvDriverResistance_R);
}

/**
  * @brief  It return latest Motor Driver NTC conversion result (raw Conversion) expressed in u16Amp
  * @param  pHandle related Handle of DriverNTC_Handle_t
  * @retval uint16_t Latest NTC conversion result in digit
  */

__weak uint16_t DRIVER_GetTemperature_d(DriverNTC_Handle_t *pHandle)
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
  * @brief  It return latest averaged MOTOR Driver NTC measurement expressed in celsius
  * @param  pHandle related Handle of DriverNTC_Handle_t
  * @retval uint16_t Latest averaged MOTOR Driver NTC measurement in Celsius
  */
__weak uint16_t DRIVER_GetAvgTemperature_d(DriverNTC_Handle_t *pHandle)
{
	return (pHandle->AvDriverTemp_s16R);
}

/**
  * @brief  It return latest averaged MOTOR Driver NTC measurement expressed in celsius
  * @param  pHandle related Handle of DriverNTC_Handle_t
  * @retval uint16_t Latest averaged MOTOR  Driver NTC measurement in Celsius
  */
__weak uint16_t DRIVER_GetAvgTemperature_C(DriverNTC_Handle_t *pHandle)
{
	return (pHandle->AvDriverTemp);
}

__weak uint16_t DRIVER_CheckTemperature(DriverNTC_Handle_t *pHandle)
{
	return (pHandle->FaultState);
}
