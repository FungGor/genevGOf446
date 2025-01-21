/*
 * ERROR_CHECK.h
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_SAFETY_PROTECTION_ERROR_REPORT_H_
#define APPLICATION_SAFETY_PROTECTION_ERROR_REPORT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include<stdint.h>

#define SOFTWARE_OK              0x00
#define ERROR_CODE_PACKET        0x4A
#define ERROR_MSG_HANDLE_LENGTH  0x01
#define SYSTEM_NORMAL            0x00
#define TRANSMISSION_TIMEOUT     0x0A
#define FATAL_SYSTEM_ERROR       0x0B

#define BATTERY_TEMP_ERROR_CODE   0x1A
#define BMS_COMM_ERROR_CODE       0x1C
#define ABNORMAL_CURRENT          0x2A
#define GATE_DRIVER_ERROR_CODE    0x2C
#define MOSFET_ERROR_CODE         0x2E
#define HALL_SENSOR_ERROR_CODE    0x3A
#define MOTOR_TEMP_ERROR_CODE     0x3C

void ERROR_REPORT_INIT();
void SOFTWARE_ERROR_REPORT_INIT();
void CHECK_MOTOR_STATUS();
void SEND_SOFTWARE_ERROR_REPORT(uint8_t fault);
void SEND_MOTOR_ERROR_REPORT(uint16_t error_code);
uint8_t GET_MOTOR_ERROR_REPORT();
uint8_t GET_SOFTWARE_ERROR_REPORT();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_SAFETY_PROTECTION_ERROR_REPORT_H_ */
