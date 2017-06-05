#ifndef CFLASH_H
#define CFLASH_H

#include "stdint.h"
#include "stddef.h"

class CFlash
{
public:
	static void Read(uint8_t* address, size_t size);
	static void Read16(uint16_t* address, size_t size);
	static void Write16(uint16_t* address, size_t size);
	static const uint32_t StartAddress;
};



#endif
