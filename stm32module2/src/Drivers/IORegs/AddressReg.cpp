#include "AddressReg.h"

AddressReg::AddressReg(IModbusSlave* pslave) :
slave(pslave)
{
	baseAddress = 0;
	offset = 1;
}

void AddressReg::Init(uint16_t baseAddress)
{
	this->baseAddress = baseAddress;
}

void AddressReg::Refresh()
{

}

bool AddressReg::IsValid(uint16_t index, uint16_t reg)
{
	if (index==this->offset)
		return true;
	return true;
}
bool AddressReg::Write(uint16_t index, uint16_t reg)
{
	return false;
}

EDeviceType AddressReg::GetDeviceType()
{
	return EDeviceType::EDeviceTypeBinInputs;
}

EModbusFunctions AddressReg::GetModbusFunc()
{
	return EModbusFunctions::EModbusReadHoldingRegisters;
}

uint16_t AddressReg::GetSizeInWords()
{
	return sizeof(uint16_t);
}

AddressReg::~AddressReg()
{

}
