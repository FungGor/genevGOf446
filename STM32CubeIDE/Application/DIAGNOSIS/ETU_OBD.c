/*
 * diagnostic_mode.c
 *
 *  Created on: Apr 16, 2025
 *      Author: TerenceLeung
 */
#include "ETU_OBD.h"
#include "flash_internal.h"  //Save Logs
#include "UDHAL_MOTOR.h"
#include <stdlib.h>

MOTOR *motorPtr;
brakeAndThrottle_t *brakeThrottleSensorPtr;

/*Save in the Flash Memory*/
uint32_t OBD_t[SIZE_OF_FLASH_PAYLOAD] ={0};
/*Stores Driver Parameters*/
uint32_t OBD_Driver[DRIVER_PARAM_PAYLOAD] = {0};
/*Stores Motor Parameters*/
uint32_t OBD_Motor[MOTOR_PARAM_PAYLOAD] = {0};
/*Stores Error Report*/
uint32_t OBD_Fault[FAULT_PAYLOAD] = {0};
/*Stores Brake and Throttle Parameters*/
uint16_t OBD_BrakeThrottleSensor[BRAKE_THROTTLE_SENSOR_PAYLOAD] = {0};

void ETU_StatusRegister(MOTOR *Motor)
{
	motorPtr = Motor;
}

void ETU__brakeThrottleSensorRegister(brakeAndThrottle_t *brakeThrottle)
{
	brakeThrottleSensorPtr = brakeThrottle;
}

/*  OBD Payload
 *  OBD_HardwareDiagnosis() is called periodically
 */
void OBD_HardwareDiagnosis()
{
   /*Check Voltage Sensors 4-Bytes*/
   OBD_t[0] = motorPtr->milliVolts;
   OBD_Driver[0] = OBD_t[0];

   calcDC();
   /*Check Current 4-Bytes*/
   OBD_t[1] = motorPtr->milliAmpere;
   OBD_Driver[1] = OBD_t[1];
   /*Check Driver Temperature 4-Bytes*/
   OBD_t[2] = motorPtr->driverTemperature;
   OBD_Driver[2] = OBD_t[2];
   /*Check Motor Temperature 4-Bytes*/
   OBD_t[3] = motorPtr->motorTemperature;
   OBD_Motor[0] = OBD_t[3];
   /*Check Speed Feedback 4-Bytes*/
   OBD_t[4] = (uint32_t)motorPtr->hallSensorState;
   OBD_Motor[1] = OBD_t[4];

   motor_speed();
   /*Check Motor Speed 4-Bytes*/
   OBD_t[5] = (uint32_t)motorPtr->SPEED;
   OBD_Motor[2] = OBD_t[5];

   CHECK_MOTOR_STATUS();
   /*Check Current Error 4-Bytes*/
   OBD_t[6] = (uint32_t)GET_MOTOR_ERROR_REPORT();
   OBD_Fault[0] = OBD_t[6];
   /*Check Software Error 4-Bytes*/
   OBD_t[7] = (uint32_t)GET_SOFTWARE_ERROR_REPORT();
   OBD_Fault[1] = OBD_t[7];
   /*Brake and Throttle Sensor*/
   OBD_BrakeThrottleSensor[0] = (uint16_t)brakeThrottleSensorPtr->IQ_applied;
   OBD_BrakeThrottleSensor[1] = brakeThrottleSensorPtr->throttlePercent;
   OBD_BrakeThrottleSensor[2] = brakeThrottleSensorPtr->throttle_voltage;
}

uint32_t* OBD_GetFullPayload()
{
	uint32_t* arr = (uint32_t*)malloc(SIZE_OF_FLASH_PAYLOAD*sizeof(uint32_t));
	for(uint8_t i = 0; i < SIZE_OF_FLASH_PAYLOAD; i++)
	{
		arr[i] = OBD_t[i];
	}
	return arr;
}

uint32_t* OBD_GetDriverPayload()
{
	uint32_t *driver = (uint32_t*)malloc(DRIVER_PARAM_PAYLOAD*sizeof(uint32_t));
	for(uint8_t i = 0; i < DRIVER_PARAM_PAYLOAD; i++)
	{
		driver[i] = OBD_Driver[i];
	}
	return driver;
}

uint32_t* OBD_GetMotorPayload()
{
	uint32_t *motor = (uint32_t*)malloc(MOTOR_PARAM_PAYLOAD*sizeof(uint32_t));
	for(uint8_t i = 0; i < MOTOR_PARAM_PAYLOAD; i++)
	{
		motor[i] = OBD_Motor[i];
	}
	return motor;
}

uint16_t* OBD_GetThrottleBrakeSensorPayload()
{
	uint16_t *ThrottleBrake = (uint16_t*)malloc(BRAKE_THROTTLE_SENSOR_PAYLOAD*sizeof(uint16_t));
	for(uint8_t i = 0; i < BRAKE_THROTTLE_SENSOR_PAYLOAD; i++)
	{
		ThrottleBrake[i] = OBD_BrakeThrottleSensor[i];
	}
	return ThrottleBrake;
}

uint32_t* OBD_GetFaultPayload()
{
	uint32_t *fault = (uint32_t*)malloc(FAULT_PAYLOAD*sizeof(uint32_t));
	for(uint8_t i = 0; i < FAULT_PAYLOAD; i++)
	{
		fault[i] = OBD_Fault[i];
	}
	return fault;
}

void RunOBD()
{
	OBD_HardwareDiagnosis();
}
