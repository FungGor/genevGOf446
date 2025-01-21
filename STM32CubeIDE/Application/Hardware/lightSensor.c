/*
 * lightSensor.c
 *
 *  Created on: Jan 17, 2025
 *      Author: TerenceLeung
 */

#include "lightSensor.h"

uint8_t lightSensor = 0xFF;

void updateLightSensorStatus(uint8_t lightStatus)
{
    lightSensor = lightStatus;
}

uint8_t getLightSensorStatus()
{
	return lightSensor;
}
