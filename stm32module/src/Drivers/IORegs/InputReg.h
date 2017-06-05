#ifndef _INPUTREG_H_
#define _INPUTREG_H_
#include <stdint.h>

#include "IOPin.h"
#include "IWriteReg.h"
#include "slave-rtu.h"

typedef struct
{
	uint16_t PinNumber:8;
	uint16_t Value:1;
	uint16_t Quality:1;
	uint16_t Latch:1;
	uint16_t LatchDir:1;
} SInputReg;

class InputReg : public IWriteReg
{
	uint8_t pinNumber;
	uint16_t offset;
	IOPin* pin;
	SlaveRtu* slave;
public:
	InputReg();
	void Init(uint16_t offset,IOPin* pin,SlaveRtu* slave,uint8_t pinNumber);
	void Refresh();
	bool IsValid(uint16_t index, uint16_t reg);
	bool Write(uint16_t index, uint16_t reg);
};


#endif
