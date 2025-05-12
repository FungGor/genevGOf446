/*
 * power_management.h
 *
 *  Created on: May 8, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MAINTASK_BACKGROUNDTASK_H_
#define APPLICATION_MAINTASK_BACKGROUNDTASK_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "cmsis_os.h"
#include "ETU_StateMachine.h"

#define POWER_TASK_DELAY       100
#define POWER_ON_INTERVAL      100

extern void etu_state_ptr_register(ETU_StateHandle_t *etuState);
extern void power_management_register(bool *ptrPower);
extern void run_background_tasks();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_BACKGROUNDTASK_H_ */
