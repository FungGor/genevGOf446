/*
 * Drive.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 *
 *      GitHub Password: xxxxxxxxxxxxxxxxxxx
 */

#ifndef APPLICATION_MAINTASK_ESCOOTER_TRANSMISSION_UNITS_H_
#define APPLICATION_MAINTASK_ESCOOTER_TRANSMISSION_UNITS_H_
#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include "stdint.h"


/***********************ESCOOTER TRANSMISSION UNITS (ETU)*************************/

// GeneralTasks_TIME = 20 milliseconds
#define GeneralTask_TIME      20

// OBDTask_TIME     = 20 milliseconds
#define OBD_TIME              20

#define N1_TIME               100
#define N2_TIME               200
#define N3_TIME               300

void go_powerOnRegister(bool *ptrpowerOn);

void go_errorReportRegister(uint8_t *report);

void software_errorReportRegister(uint8_t *fault);

/* Safety Power Cutting 2025-03-25 */
void ETU_PowerShutDown();

/* Initializes GENEV-GO core according to user defined parameters. */
void GoInit();

/*Starts Driving*/
void createDrivingTasks(void);

/* Runs all the Tasks of GENEV-GO cockpit */
void GeneralTasks(void const * argument);
#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_ESCOOTER_TRANSMISSION_UNITS_H_ */
