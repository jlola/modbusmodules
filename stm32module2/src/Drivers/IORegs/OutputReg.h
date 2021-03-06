#ifndef _OUTPUTREG_H_
#define _OUTPUTREG_H_

#include <stdint.h>
#include <stddef.h>
#include "IIOPin.h"
#include "IWriteReg.h"
#include "IModbusSlave.h"


typedef struct
{
	uint16_t PinNumber:8;
	uint16_t Value:1;
	uint16_t Quality:1;

} SOutputReg;

class OutputReg : public IWriteReg
{
	uint16_t offset;
	IIOPin* pin;
	IModbusSlave* slave;

public:
	OutputReg(IIOPin* pin,IModbusSlave* slave);
	bool Refresh();
	bool IsValid(uint16_t index, uint16_t reg);
	bool Write(uint16_t index, uint16_t reg);
	EDeviceType GetDeviceType();
	EModbusFunctions GetModbusFunc();
	uint16_t GetSizeInWords();
	void SetOffset(uint16_t offset);
	~OutputReg();
};


#endif
