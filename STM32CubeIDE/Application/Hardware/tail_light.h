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

typedef struct
{
	uint8_t lightStatus;
	uint8_t tail_light_status_old;
	uint8_t tail_light_mode;
	uint8_t tail_light_mode_old;
}Tail_Light_t;

extern void tail_light_status_Init();

extern void tail_light_toggle();

extern void set_tail_light_on();

extern void set_tail_light_off();

extern uint8_t led_indicator_on();

extern uint8_t led_indicator_off();

extern void error_indicator_on();

extern void error_indicator_off();

extern void set_tail_light_status(uint8_t status);

extern uint8_t get_tail_light_status();

extern uint8_t get_tail_light_mode();

extern uint8_t toggle_tail_light(uint8_t tailLightStatusOld);

extern void lightSensorStateChange();

extern void tailLightStateMachine();

#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_HARDWARE_TAIL_LIGHT_H_ */
