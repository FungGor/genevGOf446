/*
 * lightSensor.h
 *
 *  Created on: Jan 17, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_LIGHTSENSOR_H_
#define APPLICATION_HARDWARE_LIGHTSENSOR_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"

extern void updateLightSensorStatus(uint8_t lightStatus);

extern uint8_t getLightSensorStatus();



#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_LIGHTSENSOR_H_ */
