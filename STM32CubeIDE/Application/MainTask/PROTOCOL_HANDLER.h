/*
 * UART_Control.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MAINTASK_PROTOCOL_HANDLER_H_
#define APPLICATION_MAINTASK_PROTOCOL_HANDLER_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"

#define MAXIMUM_NUMBER_OF_LOST_PACKETS     0x0A    //15 lost packet triggers connection error 2025-03-26, time-out has been extended
/*In case the dashboard is accidentally disconnected, system stops running*/
#define TIMEOUT_EXPIRATION                 0x0A
/*Allows reconnection automatically*/
#define MAX_CHANCE_RECONNECTION            0x06

typedef struct
{
	volatile uint8_t lossPacket;          /*Packet Loss*/
	volatile uint8_t timeOutExpiration;   /*Timeout Expired*/
	volatile uint16_t payLoadReceived;    /*ACK Received Payload*/
	volatile bool stillConnection;        /*Is UART still online?*/
	volatile bool lostConnection;         /*Is UART disconnected ?*/
	volatile uint8_t reconnection;        /*Reconnect again*/
}protocol_t;
/*********************************************************************
 * @Structure STM32MCP_timerManager_t
 *
 * @brief     It defines a set of function pointer that the server
 *            wants to point to the application functions
 *
 * @data      timerStart: Called when the motor controller wants to start the timeout counter
 *            timerStop:  Called when the motor controller wants to stop the timeout counter
 */
typedef void (*STM32MCP_timerStart)(void);
typedef void (*STM32MCP_timerStop)(void);
typedef struct
{
	STM32MCP_timerStart  timerStart;
	STM32MCP_timerStop   timerStop;
}STM32MCP_protocolHandle_t;

extern void STM32MCP_registerTimer(STM32MCP_protocolHandle_t *STM32MCP_CBs);
extern void timeOutStart();
extern void timeOutStop();
extern void timeOutHandler();
extern void timeOutReset();
extern void updateConnectionStatus(bool received, uint8_t packageCount);
extern bool getConnectionStatus();
extern void checkConnectionStatus();
#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_PROTOCOL_HANDLER_H_ */
