/*
 * ETU_StateMachine.c
 *
 *  Created on: Apr 28, 2025
 *      Author: TerenceLeung
 */
#include "ETU_StateMachine.h"

/** @addtogroup MCSDK
  * @{
  */

/** @defgroup STATE_MACHINE E-Scooter Transmission Unit Machine
  * @brief E-Scooter Control State Machine component of the Motor Control SDK
  *
  * @todo Document the E-Scooter Control State Machine "module".
  *
  * @{
  */

/**
  * @brief  Initializes all the object variables, usually it has to be called
  *         once right after object creation.
  * @param  etuHandle pointer on the component instance to initialize.
  * @retval none.
  */
void ETU_Init(ETU_StateHandle_t *etuHandle)
{
	etuHandle->eState = BOOT_CHECK;
	etuHandle->Faults = ETU_NO_FAULTS;
}

/**
  * @brief It submits the request for moving the state machine into the state
  *        specified by bState (ETU_FAULTS_OCCURRED are not handled by this
  *        method). Accordingly with the current state, the command is really
  *        executed (state machine set to bState) or discarded (no state
  *        changes).
  * @param pHanlde pointer of type  ETU_StateHandle_t.
  * @param eState New requested state
  * @retval bool It returns true if the state has been really set equal to
  *         bState, false if the requested state can't be reached
  */
void ETU_NextState(ETU_StateHandle_t *etuHandle, ETU_State_t eState)
{
	bool eChangeState = false;
	ETU_State_t eCurrentState = etuHandle->eState;
	ETU_State_t eNewState = eCurrentState;

	switch( eCurrentState )
	{
	   case BOOT_CHECK:
		  if( (eState == ETU_START) || (eState == ETU_OFF_TRANSITION) || (eState == ETU_OBD) || (eState == ETU_FAULT)
			   || (eState == ETU_OFF))
		  {
			  eNewState = eState;
			  eChangeState = true;
		  }
		  break;

	   case ETU_START:
		   if((eState == ETU_OBD) || (eState == ETU_OFF) || (eState == ETU_FAULT))
		   {
			   eNewState = eState;
			   eChangeState = true;
		   }
		   break;

	   case ETU_FAULT:
		   eNewState = eState;
		   eChangeState = true;
		   break;


	   case ETU_OBD:
		   if(eState == ETU_OFF)
		   {
			   eNewState = eState;
			   eChangeState = true;
		   }
		   break;

	   case ETU_OFF_TRANSITION:
		   if(eState == ETU_OFF)
		   {
			   eNewState = eState;
			   eChangeState = true;
		   }
		   break;


	   case ETU_OFF:
		   eNewState = eState;
		   eChangeState = true;
		   break;


	   default:
		   break;
	}
	if(eChangeState)
	{
		etuHandle->eState = eNewState;
	}
	else
	{

	}
}

/**
  * @brief  Returns the current state machine state
  * @param  pHanlde pointer of type  STM_Handle_t
  * @retval State_t Current state machine state
  */
ETU_State_t ETU_GetState(ETU_StateHandle_t *etuHandle)
{
	return (etuHandle->eState);
}

void ETU_setFatalError(ETU_StateHandle_t *etuHandle, uint16_t ErrorOccurred)
{
	etuHandle->Faults = (ErrorOccurred | etuHandle->Faults);
	if(etuHandle->Faults != 0x00)
	{
		etuHandle->isFaults = ETU_FAULTS_OCCURRED;
	}
}

uint16_t ETU_getErrorState(ETU_StateHandle_t *etuHandle)
{
	return (etuHandle->isFaults);
}
