/*
 * UDHAL_TIMEOUT.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */
#include "PROTOCOL_HANDLER.h"
#include "UDHAL_TIMEOUT.h"
#include "cmsis_os.h"
#include "stdint.h"

static osTimerId protocolTimeoutHandle;
static void UDHAL_TIMEOUT_Start();
static void UDHAL_TIMEOUT_Stop();
void UDHAL_TIMEOUT_OVClockFxn(void const *argument);

static STM32MCP_protocolHandle_t timeoutHandle =
{
	UDHAL_TIMEOUT_Start,
	UDHAL_TIMEOUT_Stop
};

void UDHAL_TIMEOUT_init()
{
	/*Register Timer*/
	STM32MCP_registerTimer(&timeoutHandle);
	/*Definition and creation of timeout counter*/
	osTimerDef(timeout,UDHAL_TIMEOUT_OVClockFxn);
	protocolTimeoutHandle = osTimerCreate(osTimer(timeout),osTimerPeriodic,NULL);
}

static void UDHAL_TIMEOUT_Start()
{
	/*UDHAL_TIMEOUT_OVClockFxn is triggered in every 500 ms, 0.5s */
	osTimerStart(protocolTimeoutHandle,RECEIVE_TIMEOUT_PERIOD);
}

static void UDHAL_TIMEOUT_Stop()
{
	osTimerStop(protocolTimeoutHandle);
}

void UDHAL_TIMEOUT_OVClockFxn(void const *argument)
{
	timeOutHandler();
}
