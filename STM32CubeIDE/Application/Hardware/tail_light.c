/*
 * tail_light.c
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */
#include "tail_light.h"
#include "lightSensor.h"
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

void led_indicator_on()
{
	dashboard_indicator_on();
}

void led_indicator_off()
{
	dashboard_indicator_off();
}

void error_indicator_on()
{
	fault_warning_on();
}

void error_indicator_off()
{
	fault_warning_off();
}

void brakeStateChange()
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

void lightSensorStateChange()
{
   if(getLightSensorStatus() == 0x01)
   {
	   tail_light_turnon();
   }
   else if(getLightSensorStatus() == 0x00)
   {
	   tail_light_turnoff();
   }
}
