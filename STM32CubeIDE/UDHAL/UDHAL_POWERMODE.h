/*
 * UDHAL_POWERMODE.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef UDHAL_UDHAL_POWERMODE_H_
#define UDHAL_UDHAL_POWERMODE_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"

uint8_t GET_RESET_SOURCE();

void ShutDownTask();

void bootStart();

#ifdef __cplusplus
}
#endif
#endif /* UDHAL_UDHAL_POWERMODE_H_ */
