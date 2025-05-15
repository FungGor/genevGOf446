/*
 * tail_light.c
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */
#include "tail_light.h"
#include "lightSensor.h"
#include "brake_and_throttle.h"
#include "SleepAndWake.h"

#include "../../UDHAL/UDHAL_GPIO.h"

uint8_t lightStatus;

static uint8_t tail_light_status_old = 0;      //either: 1 = 0N    or   0 = OFF
static uint8_t tail_light_mode = ESCOOTER_TAIL_LIGHT_OFF;   // toggle = 0x05, ON = 0x08, OFF = 0x06
static uint8_t tail_light_mode_old = ESCOOTER_TAIL_LIGHT_OFF; // toggle = 0x05, ON = 0x08, OFF = 0x06

void tail_light_toggle()
{
	/*The tail light flashes when we press the brake*/
	tail_light_flash();
}

void set_tail_light_on()
{
	tail_light_turnon();
}

void set_tail_light_off()
{
    /*The tail light is turned off when we release the brake*/
	tail_light_turnoff();
}

uint8_t led_indicator_on()
{
	dashboard_indicator_on();
	return 0x01;
}

uint8_t led_indicator_off()
{
	dashboard_indicator_off();
	return 0x00;
}

void error_indicator_on()
{
	fault_warning_on();
}

void error_indicator_off()
{
	fault_warning_off();
}

void set_tail_light_status(uint8_t status)
{
	lightStatus = status;

}

uint8_t get_tail_light_status()
{
	return lightStatus;
}

uint8_t get_tail_light_mode()
{
	uint8_t mode = 0xFF;
	if(getBrakeStatus() == 0x01)
	{
		mode = ESCOOTER_TOGGLE_TAIL_LIGHT;
	}
	else if(getBrakeStatus() == 0x00)
	{
		if(lightStatus == 0x01)
		{
			mode = ESCOOTER_TAIL_LIGHT_ON;
		}
		else if(lightStatus == 0x00)
		{
			mode = ESCOOTER_TAIL_LIGHT_OFF;
		}

	}
	return mode;
}

uint8_t toggle_tail_light(uint8_t tailLightStatusOld)
{
	// toggling between tail light On and Off occurs at a rate of N2_TIME
	if(tailLightStatusOld == 0) //if tail light is OFF, toggle tail light ON
	{
		tail_light_turnon();
		tailLightStatusOld = 1;
	}
	else // if tail light is ON, toggle tail light OFF
	{
		set_tail_light_off();
		tailLightStatusOld = 0;
	}
	return tailLightStatusOld;
}

void lightSensorStateChange()
{
   if(getLightSensorStatus() == 0x01)
   {
	   tail_light_turnon();
   }
   else if(getLightSensorStatus() == 0x00)
   {
	   set_tail_light_off();
   }
}

void tailLightStateMachine()
{
	tail_light_mode = get_tail_light_mode();
	if(tail_light_mode == ESCOOTER_TOGGLE_TAIL_LIGHT)
	{
		tail_light_status_old = toggle_tail_light(tail_light_status_old);
		tail_light_mode_old = ESCOOTER_TOGGLE_TAIL_LIGHT;
	}
	else
	{
		if(tail_light_mode_old != tail_light_mode)
		{
			if(tail_light_mode == ESCOOTER_TAIL_LIGHT_OFF)
			{
				set_tail_light_off();
				tail_light_status_old = get_tail_light_status(); // either: 1 = ON   or 0 = OFF CAUTION: THIS PARAMETER IS CONTROLLED BY UART
				tail_light_mode_old = ESCOOTER_TAIL_LIGHT_OFF;
			}
			else if(tail_light_mode == ESCOOTER_TAIL_LIGHT_ON)
			{
				set_tail_light_on();
				tail_light_status_old = get_tail_light_status(); // either: 1 = ON   or 0 = OFF CAUTION: THIS PARAMETER IS CONTROLLED BY UART
				tail_light_mode_old = ESCOOTER_TAIL_LIGHT_ON;
			}
			else
			{
				// shouldn't get here
			}
		}
	}
}
