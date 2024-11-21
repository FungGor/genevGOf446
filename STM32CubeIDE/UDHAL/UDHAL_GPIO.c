/*
 * UDHAL_PIO.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */
/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
#include "stm32f4xx_hal.h"

GPIO_InitTypeDef GPIO_InitStruct = {0};
void GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB12 PB13 PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void WakeUpPinInit(void)
{
	  /*Use UART2 Rx Pin PA3 as External Interrupt Pin PORTA Pin 3 --> i.e. EXTI3*/
  GPIO_InitStruct.Pin  = GPIO_PIN_3; /*UART2 Rx Pin PA3*/
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; /*Falling Edge Triggered Interrupt by UART from CC2640 Dash-board*/
  GPIO_InitStruct.Pull = GPIO_NOPULL; /*Activate Internal Pull Up Resistor!*/
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  /*Activate Falling Edge Triggered Interrupt --> External Line Interrupt 3*/
  HAL_NVIC_SetPriority(EXTI3_IRQn,3,0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  GPIO_InitStruct.Pin  = GPIO_PIN_2; /*UART2 Tx Pin PA2*/
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_RESET);
}

void WakeUpPinDeInit(void)
{
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOA,GPIO_PIN_2);
}

void dashboard_indicator_on()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
}

void fault_warning_on()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
}

void dashboard_indicator_off()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
}

void fault_warning_off()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
}

void tail_light_flash()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_Delay(200);
}

void tail_light_turnoff()
{
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
}
