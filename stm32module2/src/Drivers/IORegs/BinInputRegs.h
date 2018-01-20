#ifndef _INPUTREG_H_
#define _INPUTREG_H_
#include <stdint.h>

#include "IOPin.h"
#include "IWriteReg.h"
#include "IModbusSlave.h"
#include "IModbusObject.h"

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
	uint16_t offset;
	IIOPin* pin;
	IModbusSlave* slave;
public:
	InputReg(IIOPin* pin, IModbusSlave* slave);
	void Refresh();
	bool IsValid(uint16_t index, uint16_t reg);
	bool Write(uint16_t index, uint16_t reg);

	EDeviceType GetDeviceType();
	EModbusFunctions GetModbusFunc();
	uint16_t GetSizeInWords();
	void SetBaseAddress(uint16_t address);
	void SetOffset(uint16_t offset);
};


#endif
