/*
 * Drive.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MAINTASK_DRIVE_TASKS_H_
#define APPLICATION_MAINTASK_DRIVE_TASKS_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include "stdint.h"

void go_powerOnRegister(bool *ptrpowerOn);

bool getPowerMode();

void go_errorReportRegister(uint8_t *report);

void software_errorReportRegister(uint8_t *fault);

/* Initializes GENEV-GO core according to user defined parameters. */
void GoInit();

void createDrivingTasks(void);

/* Runs all the Tasks of GENEV-GO cockpit */
void ES_RunDrivingTasks(void const * argument);

/* Executes safety checks (e.g. bus voltage and temperature) for all drive instances */
void ES_SafetyTask();


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_DRIVE_TASKS_H_ */
