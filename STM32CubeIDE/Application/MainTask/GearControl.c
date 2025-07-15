/*
 * Motor_Transmission.c
 *
 *  Created on: May 19, 2025
 *      Author: TerenceLeung
 */
#include <GearControl.h>

void GearInit(GearMode_Handle_t *transmission)
{
	/*E-Scooter should be in PARKING mode (N) during start-up*/
	transmission->Mode = PARK;
}

void GearToggle(GearMode_Handle_t *transmission, GearMode_t tMode)
{
	bool tChangeState = false;
	GearMode_t tCurrentMode = transmission->Mode;
	GearMode_t tNewMode = tCurrentMode;

	/*Changes Gear automatically:P N D */
	/*Toggles between DRIVE and NEUTRAL mode*/
	switch( tCurrentMode )
	{
	   case PARK:
		   if ( (tMode == NEUTRAL) )
		   {
			   tNewMode = tMode;
			   tChangeState = true;
		   }
		   break;

	   case NEUTRAL:
		   if( (tMode == DRIVE) )
		   {
			   tNewMode = tMode;
			   tChangeState = true;
		   }
		   break;

	   case DRIVE:
		   if( (tMode == NEUTRAL) )
		   {
			   tNewMode = tMode;
			   tChangeState = true;
		   }
		   break;

	   default:
		   break;

	}
	if(tChangeState)
	{
		transmission->Mode = tNewMode;
	}
	else
	{

	}
}

GearMode_t getCurrentGear(GearMode_Handle_t *transmission)
{
	return (transmission->Mode);
}

