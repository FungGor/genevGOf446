/*
 * Motor_Transmission.h
 *
 *  Created on: May 19, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MAINTASK_GEARCONTROL_H_
#define APPLICATION_MAINTASK_GEARCONTROL_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"

/*GearControl.h imitates Automatic Gear Transmission System's behaviors in the Car: P N D
 *Realization of Automatic Gear Transmission System*/

typedef enum
{
	PARK = 0,
	NEUTRAL = 1,   /*Neutral Mode --> Isolates Electric (Gear) Transmission*/
	DRIVE   = 2    /*Drive Mode --> Transmits Electric Power to the Motor (Car starts moving)*/
}GearMode_t;

typedef struct
{
	GearMode_t Mode;
}GearMode_Handle_t;


extern void GearInit(GearMode_Handle_t *transmission);

extern void GearToggle(GearMode_Handle_t *transmission, GearMode_t tMode);

GearMode_t getCurrentGear(GearMode_Handle_t *transmission);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_GEARCONTROL_H_ */
