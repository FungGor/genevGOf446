/*
 * UART_Control.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */
#include <ERROR_REPORT.h>
#include "PROTOCOL_HANDLER.h"

static STM32MCP_protocolHandle_t *STM32MCP_protocolHandle;
uint8_t packetLoss = 0x00;

/*********************************************************************
 * @fn      STM32MCP_registerTimer
 *
 * @brief   It is used to register the timer start and timer stop function to allow the library can control the peripherals
 *
 * @param   None
 *
 * @return  None
 */
void STM32MCP_registerTimer(STM32MCP_protocolHandle_t *STM32MCP_CBs)
{
	STM32MCP_protocolHandle = STM32MCP_CBs;
}

/*********************************************************************
 * @fn      timeOutStart
 *
 * @brief   Starts down-counting for time out expiration when UART is disconnected
 *
 * @param   None
 *
 * @return  None
 */
void timeOutStart()
{
	STM32MCP_protocolHandle->timerStart();
}

/*********************************************************************
 * @fn      timeOutStop
 *
 * @brief   Transmission Timeout counter is cancelled when connection is recovered
 *
 * @param   None
 *
 * @return  None
 */
void timeOutStop()
{
	STM32MCP_protocolHandle->timerStop();
}

/*********************************************************************
 * @fn      timeOutHandler
 *
 * @brief   Starts counting the number of lost packets when the connection is lost, timeout expires when there
 *          are 5 or more lost packets. timeOutHandler() sends error report to Driving task once timeout expires.
 *
 * @param   None
 *
 * @return  None
 */
void timeOutHandler()
{
	packetLoss++;
	if(packetLoss > MAXIMUM_NUMBER_OF_LOST_PACKETS)
	{
		SEND_ERROR_REPORT(TIMEOUT_EXPIRATION);
	}
}

/*********************************************************************
 * @fn      timeOutReset
 *
 * @brief   When motor controller successfully acknowledged the incoming packets from DashBoard, motor control
 *          protocol stops expiration timer and resets packetLoss back to 0x00 through calling this API.
 *
 * @param   None
 *
 * @return  None
 */
void timeOutReset()
{
    packetLoss = 0x00;
    timeOutStop();
}