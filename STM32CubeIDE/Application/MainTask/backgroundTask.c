/*
 * power_management.c
 *
 *  Created on: May 8, 2025
 *      Author: TerenceLeung
 */
#include <backgroundTask.h>
#include "SleepAndWake.h"
#include "ERROR_REPORT.h"


bool *ptrPowerStatus;
uint8_t *connectionFault;
uint32_t powerOnTime = 0;
static osThreadId idleTaskHandle;
ETU_StateHandle_t *state;

static void idleBackgroundTask(void const *argument);

void etu_state_ptr_register(ETU_StateHandle_t *etuState)
{
	state = etuState;
}

void power_management_register(bool *ptrPower)
{
	ptrPowerStatus = ptrPower;
}

void connection_behaviour_register(uint8_t *ptrConnection)
{
	connectionFault = ptrConnection;
}

void run_background_tasks()
{
	osThreadDef(idleRun,idleBackgroundTask,osPriorityIdle,0,128);
	idleTaskHandle = osThreadCreate(osThread(idleRun),NULL);
}


/*Handles with Fault and OBD --> ETU State Machine*/
uint8_t mode = 0x00;
static void idleBackgroundTask(void const *argument)
{
	for(;;)
	{
		osDelay(POWER_TASK_DELAY);
		powerOnTime += POWER_ON_INTERVAL;
		if(state->eState == ETU_FAULT)
		{
			mode = 0x01;

		}else if(state->eState == ETU_OBD){
			mode = 0x02;
		}

		if(*connectionFault != 0)
		{
			*ptrPowerStatus = false;
		}

		if(*ptrPowerStatus == false)
		{
			break;
		}
	}
	gotoSLEEP();
}
