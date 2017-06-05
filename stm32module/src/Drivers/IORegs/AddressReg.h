#define _ADDRESSREG_H_
#ifdef  _ADDRESSREG_H_

#include "IWriteReg.h"

class AddressReg : IWriteReg
{
public:
	AddressReg();
	uint16_t offset;
	bool IsValid(uint16_t index, uint16_t reg);
	bool Write(uint16_t index, uint16_t reg);
};


#endif
