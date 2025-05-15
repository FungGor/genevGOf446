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

#define CONNECT_BATTERY       0x7B
#define SKIP_DIAGNOSIS        0x00
#define ENTER_DIAGNOSIS       0x01

extern void go_powerOnRegister(bool *ptrpowerOn);

extern void go_errorReportRegister(uint8_t *report);

extern void software_errorReportRegister(uint8_t *fault);

extern void OBD_flagRegister(bool *ptrOBD);

/* Safety Power Cutting 2025-03-25 */
extern void ETU_PowerShutDown();

/* Initializes GENEV-GO core according to user defined parameters. */
extern void GoInit();

/*Starts Driving*/
extern void createDrivingTasks(void);

extern void ETU_BootRoutine(uint8_t diagnosis);

extern void gearReady();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_ESCOOTER_TRANSMISSION_UNITS_H_ */
