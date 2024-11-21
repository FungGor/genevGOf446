/*
 * UDHAL_UART.c
 *
 *  Created on: Sep 3, 2024
 *      Author: TerenceLeung
 */

#include "../UDHAL/UDHAL_UART.h"
#include "../UDHAL/UDHAL_GPIO.h"
#include "main.h"

UART_HandleTypeDef huart1;
uint8_t UART_OFF = 0x00;

void UART_Init()
{
	  huart1.Instance = USART1;
	  huart1.Init.BaudRate = 115200;
	  huart1.Init.WordLength = UART_WORDLENGTH_8B;
	  huart1.Init.StopBits = UART_STOPBITS_1;
	  huart1.Init.Parity = UART_PARITY_NONE;
	  huart1.Init.Mode = UART_MODE_TX_RX;
	  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart1) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

uint8_t UART_DeInit()
{
    if(HAL_UART_DeInit(&huart1) == HAL_OK)
    {
    	UART_OFF = 0x01;
    }
    else if(HAL_UART_DeInit(&huart1) != HAL_OK)
    {
    	UART_OFF = 0xFF;
    }
    return UART_OFF;
}
