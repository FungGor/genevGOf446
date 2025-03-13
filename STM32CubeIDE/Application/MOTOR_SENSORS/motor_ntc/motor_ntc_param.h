/*
 * motor_ntc_param.h
 *
 *  Created on: Mar 3, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_PARAM_H_
#define APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_PARAM_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "stdint.h"
#include "parameters_conversion.h"

#define SUPPLY_VOLTAGE                        5       //Motor NTC Supply Voltage
#define ADC_VOLTAGE_RANGE                     3.30    //Vdd
#define RESISTANCE_OUTPUT                     (float)5100    //R4
#define RESISTANCE_INPUT                      (float)1500    //R3
#define RESISTANCE_MOTOR                      (float)10000   //R2
#define ALPHA                                 (float)((RESISTANCE_INPUT+RESISTANCE_OUTPUT)/RESISTANCE_OUTPUT)
#define EQUIVALENT_RESISTANCE                 (float)(1/((1/(RESISTANCE_INPUT+RESISTANCE_OUTPUT)) + (1/RESISTANCE_MOTOR)))
#define MOTOR_TEMP_BANDWIDTH_FACTOR           200
#define CONVERSION_RANGE                      65536
#define MOTOR_TEMP_SAMPLING                   LL_ADC_SAMPLING_CYCLE(3)




#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif /* APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_PARAM_H_ */
