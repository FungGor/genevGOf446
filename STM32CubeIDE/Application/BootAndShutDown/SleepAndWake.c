/*
 * SleepAndWake.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#include "SleepAndWake.h"
#include "drive_tasks.h"
#include "UDHAL_POWERMODE.h"

static bool POWER_ON = true;

void powerModeInit()
{
	go_powerOnRegister(&POWER_ON);
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
