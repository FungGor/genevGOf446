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

/*********************************************************************************************
 *  Constants
 *********************************************************************************************/
#define TEMP0               298.15
#define RESISTANCE0         10000
#define BCONSTANT_AV        3425.5
#define UPPER_RESISTANCE    251200  // correspond to -40 degree C
#define LOWER_RESISTANCE    500     // correspond to 130 degree C
#define ABSOLUTE_TEMP       273.15

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
	RegConv_t driverTemp;
}DriverTemp_Handle_t;

void MOTORDRIVERTEMP_Init(DriverTemp_Handle_t *pHandle);
void MOTORDRIVERTEMP_Clear(DriverTemp_Handle_t *pHandle);
uint32_t MOTORDRIVERTEMP_CalcAvR_Value(DriverTemp_Handle_t *pHandle);
extern int heatSinkTempOffset50C(uint32_t R_value);


#ifdef _cplusplus
}
#endif






#endif /* HARDWARE_NTCG163JF103FT_H_ */
