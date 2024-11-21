/*
 * UDHAL_TIMEOUT.h
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

#ifndef UDHAL_UDHAL_TIMEOUT_H_
#define UDHAL_UDHAL_TIMEOUT_H_
#ifdef __cplusplus
extern "C"{
#endif

/*Software Timer with Periodic Timer is used for handling transmission flow between motor controller and dash-board
 *
 * When the motor controller loses communication, UDHAL_TIMEOUT is triggered.
 * */
#define RECEIVE_TIMEOUT_PERIOD    500

extern void UDHAL_TIMEOUT_init();

#ifdef __cplusplus
}
#endif
#endif /* UDHAL_UDHAL_TIMEOUT_H_ */
