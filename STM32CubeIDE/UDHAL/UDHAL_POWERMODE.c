/*
 * UDHAL_POWERMODE.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#include "../UDHAL/UDHAL_POWERMODE.h"

#include "stm32f4xx_hal.h"

#include "../UDHAL/UDHAL_GPIO.h"
#include "../UDHAL/UDHAL_UART.h"
#include "../UDHAL/UDHAL_SYSCLK.h"

uint8_t rst_source;
void GET_RESET_SOURCE()
{
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_LPWRRST))
	{
		rst_source = 0x7F;
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
	{
		rst_source = 0x7C;
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
	{
		rst_source = 0x7B;
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_BORRST))
	{
		rst_source = 0x79;
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST))
	{
		rst_source = 0x7E;
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
	{
		rst_source = 0x7D;
	}
}

void ShutDownTask()
{
    if(UART_DeInit() == 0x01)
    {
    	WakeUpPinInit();
    	//dashboard_indicator_off();
    	//fault_warning_off();
    	//tail_light_turnoff();
    	/*Suspend Retransmission Timer*/


    	/*Suspend the RTOS tasks (Not Defined RTOS Tasks yet)*/


    	/*Suspend System Ticks*/
    	HAL_SuspendTick();
        /*Starts Power Off*/
    	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON,PWR_STOPENTRY_WFI);
    }
    else if(UART_DeInit() == 0xFF)
    {
        /*Fuck Off*/
    	/*System Shutdown Fails*/
    	/*What Happens if UART cannot de-init ??*/
    }
}

void bootStart()
{
	/* Reset all the system peripherals ! Reset the program counter! You could de-active EXTI First
	 * Resume RTOS tasks !
	 * */
	GET_RESET_SOURCE();
	WakeUpPinDeInit();
	SystemClock();
	HAL_ResumeTick();
	HAL_NVIC_SystemReset();
}
