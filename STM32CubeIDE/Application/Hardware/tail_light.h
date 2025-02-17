/*
 * tail_light.h
 *
 *  Created on: 2 Sep 2024
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_HARDWARE_TAIL_LIGHT_H_
#define APPLICATION_HARDWARE_TAIL_LIGHT_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "stdint.h"


#define ESCOOTER_TOGGLE_TAIL_LIGHT       0x05
#define ESCOOTER_TAIL_LIGHT_OFF          0x06
#define ESCOOTER_TAIL_LIGHT_ON           0x08

void tail_light_init();

void tail_light_toggle();

void set_tail_light_on();

void set_tail_light_off();

uint8_t led_indicator_on();

uint8_t led_indicator_off();

void error_indicator_on();

void error_indicator_off();

void set_tail_light_status(uint8_t status);

uint8_t get_tail_light_status();

uint8_t get_tail_light_mode();

uint8_t toggle_tail_light(uint8_t tailLightStatusOld);

void lightSensorStateChange();


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_TAIL_LIGHT_H_ */
