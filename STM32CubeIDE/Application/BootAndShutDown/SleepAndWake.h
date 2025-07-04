/*
 * SleepAndWake.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_BOOTANDSHUTDOWN_SLEEPANDWAKE_H_
#define APPLICATION_BOOTANDSHUTDOWN_SLEEPANDWAKE_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"

extern void powerModeInit();
extern void powerManagementInit();
extern void changePowerMode();
extern void gotoSLEEP();
extern void wakeUp();
extern uint8_t getBootSource();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_BOOTANDSHUTDOWN_SLEEPANDWAKE_H_ */
