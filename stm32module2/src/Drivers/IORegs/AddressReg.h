#define _ADDRESSREG_H_
#ifdef  _ADDRESSREG_H_

#include "Settings.h"
#include "IWriteReg.h"
#include "IModbusObject.h"
#include "IModbusSlave.h"


class AddressReg : public IWriteReg
{
	IModbusSlave* slave;
public:
	void Init(uint16_t baseAddress);
	AddressReg(IModbusSlave* pslave);
	uint16_t baseAddress;
	uint16_t offset;
	bool Refresh();
	bool IsValid(uint16_t index, uint16_t reg);
	bool Write(uint16_t index, uint16_t reg);
	virtual ~AddressReg();
	EDeviceType GetDeviceType();
	EModbusFunctions GetModbusFunc();
	uint16_t GetSizeInWords();
};


#endif
