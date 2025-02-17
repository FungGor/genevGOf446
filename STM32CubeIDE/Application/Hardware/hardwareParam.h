/*
 * hardwareParam.h
 *
 *  Created on: Nov 27, 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_HARDWAREPARAM_H_
#define APPLICATION_HARDWARE_HARDWAREPARAM_H_
#ifdef __cplusplus
extern "C"{
#endif

#undef MOTOR_CONTROL
#define DEBUG 1
#define VDD_SUPPLY            3.3
#define ADC_RANGE             65536
#define CURRENT_SENSOR_GAIN   0.04
#define RMS_FACTOR            1.41421356237
#define CURRENT_SAMPLES       240

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_HARDWAREPARAM_H_ */
