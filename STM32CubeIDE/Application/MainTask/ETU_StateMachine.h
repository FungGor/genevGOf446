/*
 * ETU_StateMachine.h
 *
 *  Created on: Apr 28, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MAINTASK_ETU_STATEMACHINE_H_
#define APPLICATION_MAINTASK_ETU_STATEMACHINE_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"
#include "stdbool.h"
#include "mc_type.h"

#define ETU_NO_FAULTS           (uint16_t)(0x0000u)
#define ETU_FAULTS_OCCURRED     (uint16_t)(0x0001u)

typedef enum
{
	BOOT_CHECK         =  0,  /*Boot Checking --> Hardware Health Diagnosis*/
	ETU_START          =  1,  /*Start E-Scooter Driving*/
	ETU_FAULT          =  2,  /*E-Scooter Hardware / Software Fault*/
	ETU_OBD            =  3,  /*Enters E-Scooter Diagnosis Mode*/
	ETU_OFF            =  4   /*The End of E-Scooter's Driving Task*/
}ETU_State_t;

typedef struct
{
	ETU_State_t eState;

	uint16_t Faults;

	uint16_t isFaults;

}ETU_StateHandle_t;

/*Initialize all the component variables*/
void ETU_Init(ETU_StateHandle_t *etuHandle);

/* It submits the request for moving the state machine into the state bState */
void ETU_NextState(ETU_StateHandle_t *etuHandle, ETU_State_t eState);

/* Returns the current state machine state */
ETU_State_t ETU_GetState(ETU_StateHandle_t *etuHandle);

void ETU_setFatalError(ETU_StateHandle_t *etuHandle, uint16_t ErrorOccurred);

uint16_t ETU_getErrorState(ETU_StateHandle_t *etuHandle);

/**
  * It returns two 16 bit fields containing information about both faults
  * currently present and faults historically occurred since the state
  * machine has been moved into state
  */
uint32_t ETU_GetFaultState(ETU_StateHandle_t *etuHandle);

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_ETU_STATEMACHINE_H_ */
