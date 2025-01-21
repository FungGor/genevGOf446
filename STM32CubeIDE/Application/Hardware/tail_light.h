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

void tail_light_init();

void tail_light_toggle();

void tail_light_off();

void led_indicator_on();

void led_indicator_off();

void error_indicator_on();

void error_indicator_off();

void brakeStateChange();

void lightSensorStateChange();


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_TAIL_LIGHT_H_ */
