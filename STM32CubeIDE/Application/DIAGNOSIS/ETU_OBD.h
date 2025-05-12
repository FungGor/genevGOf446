/*
 * diagnostic_mode.h
 *
 *  Created on: Apr 16, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_DIAGNOSIS_ETU_OBD_H_
#define APPLICATION_DIAGNOSIS_ETU_OBD_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"
#include "motor_param.h"
#include "brake_and_throttle.h" //Get Brakes + Throttle Sensor Operation Voltage
#include "mc_tasks.h"
#include "hall_speed_pos_fdbk.h" //Copy the backbone static HALL_Init_Electrical_Angle() from hall_speed_pos_fdbk.c to check Hall Sensor Status
#include "ERROR_REPORT.h"

#define SIZE_OF_FLASH_PAYLOAD           0x08
#define DRIVER_PARAM_PAYLOAD            0x03
#define MOTOR_PARAM_PAYLOAD             0x03
#define FAULT_PAYLOAD                   0x02
#define BRAKE_THROTTLE_SENSOR_PAYLOAD   0x03

/*On-Board Diagnosis Mode of ETU*/
extern void ETU_StatusRegister(MOTOR *Motor);
/*On-Board Diagnosis --> Brake and Throttle*/
extern void ETU__brakeThrottleSensorRegister(brakeAndThrottle_t *brakeThrottle);
/*Checks Hall Sensor, Driver NTC, Motor NTC, Current Sensor, Voltage Sensor's functionality + Periodically Collect Brake + Accelerator Sensors' data*/
extern void OBD_HardwareDiagnosis();
/*Get OBD Full Payload*/
extern uint32_t* OBD_GetFullPayload();
/*Get OBD Driver Payload*/
extern uint32_t* OBD_GetDriverPayload();
/*Get OBD Motor Payload*/
extern uint32_t* OBD_GetMotorPayload();
/*Get OBD Throttle and Brake Sensor Payload*/
extern uint16_t* OBD_GetThrottleBrakeSensorPayload();
/*Get OBD Fault Payload*/
extern uint32_t* OBD_GetFaultPayload();
/*Get OBD Status--> Ready for OBD or Ready for Exiting OBD ??*/
extern bool getOBDStatus();
/*Run OBD Mode*/
extern void RunOBD();


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_DIAGNOSIS_ETU_OBD_H_ */
