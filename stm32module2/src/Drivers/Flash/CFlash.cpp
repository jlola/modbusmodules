#include "stm32f0xx_flash.h"
#include "CFlash.h"
#include "string.h"




const uint32_t CFlash::StartAddress = 0x08007C00;

void CFlash::Read(uint8_t* address, size_t size)
{
	memcpy(address,(void*)CFlash::StartAddress,size);
}

void CFlash::Read16(uint16_t* address, size_t size)
{
	memcpy(address,(void*)CFlash::StartAddress,size*sizeof(uint16_t));
}

void CFlash::Write16(uint16_t* address, size_t size)
{
	FLASH_Unlock();
//	/* Clear All pending flags */
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPERR);
	FLASH_ErasePage(StartAddress);

//
	for(size_t i=0;i<size;i+=1)
	{
		uint16_t val = address[i];
		//FLASH_ProgramWord((uint32_t)(StartAddress + i*4),val);
		FLASH_ProgramHalfWord((uint32_t)(StartAddress + i*2),val);
	}

	FLASH_Lock();
}

