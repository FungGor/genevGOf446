#include "flash_internal.h"
#include "UDHAL_FLASH.h"

void flash_unlock()
{
	FLASH_ENABLE();
}

void flash_lock()
{
	FLASH_DISABLE();
}

void flash_internal_save(uint32_t addrPtr, uint32_t* DATA, uint16_t numberofWords)
{
	FLASH_SAVE(addrPtr,DATA,numberofWords);
}

uint32_t* flash_internal_load(uint32_t startSectorADDR, uint16_t numberofwords)
{
	uint32_t* readBuffer = FLASH_READ(startSectorADDR,numberofwords);
	return readBuffer;
}

void flash_reset(uint32_t Sector, uint8_t VoltageRange)
{
	FLASH_ERASE(Sector,VoltageRange);
}
