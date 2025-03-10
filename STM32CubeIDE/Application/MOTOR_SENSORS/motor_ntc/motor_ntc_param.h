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
#define RESISTANCE_OUTPUT                     5100    //R4
#define RESISTANCE_INPUT                      1500    //R3
#define RESISTANCE_MOTOR                      10000   //R2
#define RESISTANCE_DIVIDER                    20000
#define MOTOR_TEMP_BANDWIDTH_FACTOR           200
#define CONVERSION_RANGE                      65536
#define MOTOR_TEMP_SAMPLING                   LL_ADC_SAMPLING_CYCLE(3)




#ifdef __cplusplus
}
#endif /* __cpluplus */
#endif /* APPLICATION_MOTOR_SENSORS_MOTOR_NTC_MOTOR_NTC_PARAM_H_ */
