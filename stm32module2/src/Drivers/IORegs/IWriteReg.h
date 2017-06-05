#ifndef _IWRITEREG_H_
#define _IWRITEREG_H_

#include <stdint.h>

class IWriteReg
{
public:
	virtual bool IsValid(uint16_t index, uint16_t reg)=0;

	virtual bool Write(uint16_t index, uint16_t reg)=0;
};





#endif
