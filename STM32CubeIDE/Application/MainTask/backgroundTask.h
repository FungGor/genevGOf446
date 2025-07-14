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
#include "ETU_OBD.h"
#include "escooter_transmission_units.h"
#include "PROTOCOL_HANDLER.h"
#include "ERROR_REPORT.h"

#define POWER_TASK_DELAY       100
#define POWER_ON_INTERVAL      100

#define T1_TIME                200
#define T2_TIME                300
#define T3_TIME                400

typedef struct
{
	bool *ptrBackgroundPower;
	uint8_t *ptrBackgroundConnection;
	bool OBD;
}BackgroundTask_Handle_t;

extern void etu_state_ptr_register(ETU_StateHandle_t *etuState);
extern void power_management_register(bool *ptrPower);
extern void connection_behaviour_register(uint8_t *ptrConnection);
extern void hardware_check();
extern void connection_timeout_check();
extern void OBDSignalInit();
extern void enterOBD();
extern bool OBD_PIN_Checking(); /*To enter OBD, PIN is necessary*/
extern void run_background_tasks();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_BACKGROUNDTASK_H_ */
