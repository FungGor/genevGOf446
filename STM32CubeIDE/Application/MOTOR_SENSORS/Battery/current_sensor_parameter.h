/*
 * current_sensor_parameter.h
 *
 *  Created on: Feb 18, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_MOTOR_SENSORS_BATTERY_CURRENT_SENSOR_PARAMETER_H_
#define APPLICATION_MOTOR_SENSORS_BATTERY_CURRENT_SENSOR_PARAMETER_H_

#include "parameters_conversion.h"

#define SHUNT_RESISTANCE              0.005
#define CURRENT_GAIN                  25
#define AMPLIFY_GAIN                  SHUNT_RESISTANCE*CURRENT_GAIN
#define M1_CBUS_SW_FILTER_BW_FACTOR   200

#define CURRENT_SAMPLING              LL_ADC_SAMPLING_CYCLE(3)
#endif /* APPLICATION_MOTOR_SENSORS_BATTERY_CURRENT_SENSOR_PARAMETER_H_ */
