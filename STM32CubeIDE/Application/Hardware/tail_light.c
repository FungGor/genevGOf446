/*
 * tail_light.c
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */
#include "tail_light.h"
#include "brake_and_throttle.h"

#include "../../UDHAL/UDHAL_GPIO.h"

void tail_light_init()
{
    /*The tail light is off*/
	//GPIO_Init();
}

void tail_light_toggle()
{
	/*The tail light flashes when we press the brake*/
	tail_light_flash();
}

void tail_light_off()
{
    /*The tail light is turned off when we release the brake*/
	tail_light_turnoff();
}

void lightStateChange()
{
	if(getBrakeStatus() == true)
	{
		tail_light_toggle();
	}
	else if(getBrakeStatus() == false)
	{
		tail_light_off();
	}
}
