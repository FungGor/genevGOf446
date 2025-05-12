/*
 * UDHAL_FLASH.c
 *
 *  Created on: Apr 16, 2025
 *      Author: TerenceLeung
 */
#include "UDHAL_FLASH.h"


static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t SECTOR7 = 0x08060000;
uint32_t currPtr = 0x00;
uint32_t startPtr = 0x00;

void setADDptr(uint32_t addr)
{
    currPtr = addr;
}

uint32_t getADDptr()
{
	return currPtr;
}

void setStartAddress(uint32_t ptr)
{
	startPtr = ptr;
}

uint32_t getStartAddress()
{
	return startPtr;
}

uint32_t GetSector(uint32_t ADDRESS)
{
	uint32_t SECTOR = 0xFF;
	if(ADDRESS >= 0x08060000 && ADDRESS < 0x0807FFFF)
	{
		SECTOR = 0x07;
	}
	else if(ADDRESS >= 0x08040000 && ADDRESS < 0x0805FFFF)
	{
		SECTOR = 0x06;
	}
	else if(ADDRESS >= 0x08020000 && ADDRESS < 0x0803FFFF)
	{
		SECTOR = 0x05;
	}
	else if(ADDRESS >= 0x08010000 && ADDRESS < 0x0801FFFF)
	{
		SECTOR = 0x04;
	}
	else if(ADDRESS >= 0x0800C000 && ADDRESS < 0x0800FFFF)
	{
		SECTOR = 0x03;
	}
	else if(ADDRESS >= 0x08008000 && ADDRESS < 0x0800BFFF)
	{
		SECTOR = 0x02;
	}
	else if(ADDRESS == 0x08004000 && ADDRESS < 0x08007FFF)
	{
		SECTOR = 0x01;
	}
	else if(ADDRESS == 0x08000000 && ADDRESS < 0x08003FFF)
	{
		SECTOR = 0x00;
	}
	return SECTOR;
}

__weak void FLASH_ENABLE()
{
	HAL_FLASH_Unlock();
}

__weak void FLASH_DISABLE()
{
	HAL_FLASH_Lock();
}

/* Byte       = uint8_t 8-bit               --> ADDR = ADDR + 1   END_ADDR = ADDR + 1*INDEX
 * HalfWord   = 2 consecutive bytes 16-bit  --> ADDR = ADDR + 2   END_ADDR = ADDR + 2*INDEX
 * Word       = 4 consecutive bytes 32-bit  --> ADDR = ADDR + 4   END_ADDR = ADDR + 4*INDEX
 * DoubleWord = 8 consecutive bytes 64-bit  --> ADDR = ADDR + 8
 */
__weak uint32_t FLASH_SAVE(uint32_t addrPtr, uint32_t* DATA, uint16_t numberofWords)
{
	setStartAddress(addrPtr);
	int index = 0;

    HAL_FLASH_Unlock();

    uint32_t StartSector = GetSector(addrPtr);
    uint32_t EndSectorAddress = addrPtr + numberofWords*4;
    uint32_t EndSector = GetSector(addrPtr);

    EraseInitStruct.TypeErase    = FLASH_TYPEERASE_SECTORS;
    EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    EraseInitStruct.Sector       = StartSector;
    EraseInitStruct.NbSectors    = (EndSector-StartSector) + 1;

    //if (HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
    //{
    //	return HAL_FLASH_GetError();
    //}

    while(index < numberofWords)
    {
    	if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addrPtr, DATA[index]) == HAL_OK)
    	{
    		addrPtr += 4;
    		index++;
    	}
    	else
    	{
    		return HAL_FLASH_GetError();
    	}
    }
    setADDptr(addrPtr);
    HAL_FLASH_Lock();
    return 0;
}

uint32_t* FLASH_READ(uint32_t startSectorADDR, uint16_t numberofwords)
{
	uint32_t* rxBuffer = (uint32_t*)malloc(numberofwords*sizeof(uint32_t));

	for(uint16_t i = 0; i<=numberofwords; i++)
	{
		rxBuffer[i] = *(__IO uint32_t *)startSectorADDR;
		startSectorADDR += 4;
	}
	return rxBuffer;
}

__weak void FLASH_ERASE(uint32_t Sector, uint8_t VoltageRange)
{
	/*Unlock the Flash*/
	HAL_FLASH_Unlock();
	/*Erase the Sector*/
	FLASH_Erase_Sector(Sector,VoltageRange);
	/*Lock the Flash*/
	HAL_FLASH_Lock();
}
