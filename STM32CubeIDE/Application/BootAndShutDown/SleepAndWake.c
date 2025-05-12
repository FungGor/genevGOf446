/*
 * SleepAndWake.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#include <backgroundTask.h>
#include "escooter_transmission_units.h"
#include "SleepAndWake.h"
#include "UDHAL_POWERMODE.h"

static bool POWER_ON = true;

void powerModeInit()
{
	go_powerOnRegister(&POWER_ON);
}

void powerManagementInit()
{
	power_management_register(&POWER_ON);
}

void changePowerMode()
{
	if(POWER_ON == true)
	{
		POWER_ON = false;
	}
}

void gotoSLEEP()
{
	ShutDownTask();
}

void wakeUp()
{
	bootStart();
}

uint8_t getBootSource()
{
	uint8_t Boot = GET_RESET_SOURCE();
	return Boot;
}
