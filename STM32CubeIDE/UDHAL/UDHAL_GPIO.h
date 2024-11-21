/*
 * UDHAL_GPIO.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef UDHAL_UDHAL_GPIO_H_
#define UDHAL_UDHAL_GPIO_H_

#ifdef __cplusplus
extern "C"{
#endif


extern void GPIO_Init(void);

extern void WakeUpPinInit(void);

extern void WakeUpPinDeInit(void);

extern void dashboard_indicator_on();

extern void fault_warning_on();

extern void dashboard_indicator_off();

extern void fault_warning_off();

extern void tail_light_flash();

extern void tail_light_turnoff();

#ifdef __cplusplus
}
#endif
#endif /* UDHAL_UDHAL_GPIO_H_ */
