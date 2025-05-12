/*
 * flash_internal.h
 *
 *  Created on: Apr 16, 2025
 *      Author: TerenceLeung
 */

#ifndef APPLICATION_FLASH_MEMORY_FLASH_INTERNAL_H_
#define APPLICATION_FLASH_MEMORY_FLASH_INTERNAL_H_
#ifdef __cplusplus
extern "C"{
#endif

#include "stdbool.h"
#include "stdint.h"

#define SECTOR7
#undef  SECTOR6
#undef  SECTOR5

extern void flash_unlock();

extern void flash_lock();

extern void flash_internal_save(uint32_t addrPtr, uint32_t* DATA, uint16_t numberofWords);

extern uint32_t* flash_internal_load(uint32_t startSectorADDR, uint16_t numberofwords);

extern void flash_reset(uint32_t Sector, uint8_t VoltageRange);


#ifdef __cplusplus
}
#endif
#endif /* APPLICATION_FLASH_MEMORY_FLASH_INTERNAL_H_ */
