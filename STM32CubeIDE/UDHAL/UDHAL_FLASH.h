/*
 * UDHAL_FLASH.h
 *
 *  Created on: Apr 16, 2025
 *      Author: TerenceLeung
 */

#ifndef UDHAL_UDHAL_FLASH_H_
#define UDHAL_UDHAL_FLASH_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "main.h"
#include "stdint.h"
#include "stdlib.h"

extern void setADDptr(uint32_t addr);

extern uint32_t getADDptr();

extern void setStartAddress(uint32_t ptr);

extern uint32_t getStartAddress();

extern uint32_t GetSector(uint32_t ADDRESS);

extern void FLASH_ENABLE();

extern void FLASH_DISABLE();

extern uint32_t FLASH_SAVE(uint32_t addrPtr, uint32_t* DATA, uint16_t numberofWords);

extern uint32_t* FLASH_READ(uint32_t startSectorADDR, uint16_t numberofwords);

extern void FLASH_ERASE(uint32_t Sector, uint8_t VoltageRange);


#ifdef __cplusplus
}
#endif
#endif /* UDHAL_UDHAL_FLASH_H_ */
