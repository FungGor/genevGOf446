/*
 * motor_ntc.c
 *
 *  Created on: Feb 28, 2025
 *      Author: TerenceLeung
 */
#include "motor_ntc.h"


/**
  * @brief  It return latest converted MOTOR NTC resistance measurement expressed in u32
  * @param  pHandle related Handle of MotorNTCSensor_Handle_t
  * @retval uint32_t Latest averaged MOTOR NTC resistance measurement in Resistance
  */
__weak uint32_t MOTOR_GetAvgNTCResistance_R(MotorNTCSensor_Handle_t *pHandle)
{
	return (pHandle->AvResistance_R);
}
/**
  * @brief  It return latest Motor NTC conversion result (raw Conversion) expressed in u16Amp
  * @param  pHandle related Handle of MotorNTCSensor_Handle_t
  * @retval uint16_t Latest NTC conversion result in digit
  */
__weak uint16_t MOTOR_GetTemperature_d(MotorNTCSensor_Handle_t *pHandle)
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
  * @brief  It return latest averaged MOTOR NTC measurement expressed in u16
  * @param  pHandle related Handle of MotorNTCSensor_Handle_t
  * @retval uint16_t Latest averaged MOTOR NTC measurement in digit
  */
__weak uint16_t MOTOR_GetAvgTemperature_d(MotorNTCSensor_Handle_t *pHandle)
{
	return (pHandle->AvTemp_s16R);
}

/**
  * @brief  It return latest averaged MOTOR NTC measurement expressed in celsius
  * @param  pHandle related Handle of MotorNTCSensor_Handle_t
  * @retval uint16_t Latest averaged MOTOR NTC measurement in Celsius
  */
__weak uint16_t MOTOR_GetAvgTemperature_C(MotorNTCSensor_Handle_t *pHandle)
{
	return (pHandle->AvTemp);
}

__weak uint16_t MOTOR_CheckTemperature(MotorNTCSensor_Handle_t *pHandle)
{
	return (pHandle->FaultState);
}
