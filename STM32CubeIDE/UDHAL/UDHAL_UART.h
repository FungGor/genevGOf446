/*
 * UDHAL_UART.h
 *
 *  Created on: Sep 3, 2024
 *      Author: TerenceLeung
 */

#ifndef UDHAL_UDHAL_UART_H_
#define UDHAL_UDHAL_UART_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

void UART_Init();

uint8_t UART_DeInit();

#ifdef __cplusplus
}
#endif
#endif /* UDHAL_UDHAL_UART_H_ */
