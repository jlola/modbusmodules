#include "AddressReg.h"

AddressReg::AddressReg()
{
	offset = 1;
}

bool AddressReg::IsValid(uint16_t index, uint16_t reg)
{
	if (index==this->offset)
		return true;
	return true;
}
bool AddressReg::Write(uint16_t index, uint16_t reg)
{

}
