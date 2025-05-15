/*
 * power_management.c
 *
 *  Created on: May 8, 2025
 *      Author: TerenceLeung
 */
#include <backgroundTask.h>
#include "SleepAndWake.h"

static char *password = "12345678";
static bool *ptrPowerStatus;
static uint8_t *connectionFault;
static uint32_t powerOnTime = 0;
static osThreadId idleTaskHandle;
static ETU_StateHandle_t *state;
static bool OBD = false;

static uint8_t T1_TICK = T1_TIME/POWER_TASK_DELAY;
static uint8_t T2_TICK = T2_TIME/POWER_TASK_DELAY;
static uint8_t T3_TICK = T3_TIME/POWER_TASK_DELAY;

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


void hardware_check()
{
	RunOBD();
}

void connection_timeout_check()
{
   if(*connectionFault != 0)
   {
	  *ptrPowerStatus = false;
   }
}

void OBDFlagInit()
{
	OBD_flagRegister(&OBD);
}

void enterOBD()
{
	if(OBD == false){OBD = true;}
}

/*Handles with Fault and OBD --> ETU State Machine*/
/*Runs SYSTEM Diagnosis  ??? --> create pointer (share variable with escooter_transmission_unit library) ???*/
uint8_t mode = 0x00;
static void idleBackgroundTask(void const *argument)
{
	for(;;)
	{
		osDelay(POWER_TASK_DELAY);
		powerOnTime += POWER_ON_INTERVAL;
		if(powerOnTime == UINT32_MAX)powerOnTime = 0;

		/*Handles Error*/
		if(state->eState == ETU_FAULT)
		{
			mode = 0x01;

		}else if(state->eState == ETU_OBD){
			mode = 0x02;
			/*TIMEOUT Checking should be given for disconnection handling*/
			if( (powerOnTime % T1_TICK) == 0 )
			{
				hardware_check();
			}
		}
		/*Sees if there's any connection fault, if yes, *ptrPowerStatus is toggled to false which triggers auto shut-down*/
		connection_timeout_check();
		if(*ptrPowerStatus == false)break;
	}
	gotoSLEEP();
}
