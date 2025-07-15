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

#define MOTOR_CONTROL_AVAILABLE  1

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
#define DECISION_MAKER        0x03
#define FAULT_RECORD          0x05

/*ETU_Status_Handle_t handles all ETU's system status*/
typedef struct
{
	bool *ptrPower;                   /*POWER ON/OFF Status*/
	uint8_t *ptrMotorFault;           /*MOTOR Status*/
	uint8_t *ptrConnectFault;         /*UART Connection Status*/
	bool *OBD;                        /*OBD Status*/
	uint8_t isPowerOnBoot;            /*Power By the Battery?*/
	uint8_t inConnectionLED;          /*LED Indicator Status--> DashboardConnected? 1:0*/
	uint8_t DriveDecisionMaker[3];    /*Decides whether the E-Scooter could start engine transmission*/
	uint8_t gearStop;                 /*Signals if Gear Transmission is ready which determines Transmission Mode*/
	uint8_t isFault;                  /*The Fault Flag is raised to 1 if any kinds of faults occurs including UART Fault*/
	uint8_t ETU_Fault_Record[5];      /*Saves ETU Fault Record*/
}ETU_Status_Handle_t;


extern void power_flag_register(bool *ptrShutDownFlag);

extern void motor_fault_signal(uint8_t *ptrMotorFaultFlag);

extern void connection_flag_register(uint8_t *ptrConnectionFlag);

extern void obd_mode_flag_register(bool *ptrOBDFlag);

extern void ETU_StatusHandlerInit();

/* Initializes GENEV-GO core according to user defined parameters. */
extern void GoInit();

/*Starts Driving*/
extern void createDrivingTasks(void);

extern void ETU_BootRoutine(uint8_t diagnosis);

extern void saveFaultRecord(uint8_t errorCode);

/* Safety Power Cutting 2025-03-25 */
extern void ETU_PowerShutDown();

extern void ETU_connectionFaultHandler();

extern void ETU_GearTransmitStart();

extern void ETU_SignalIndicatorOFF();

extern void ETU_FaultHandler();

extern uint8_t gearAvailable();

extern void GearTransmitControlPanel();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_MAINTASK_ESCOOTER_TRANSMISSION_UNITS_H_ */
