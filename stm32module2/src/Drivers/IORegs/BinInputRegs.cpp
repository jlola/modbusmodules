#ifndef UNITTEST

#include "BinInputRegs.h"

InputReg::InputReg(IIOPin* pin, IModbusSlave* slave)
{

	this->pin = pin;
	this->slave = slave;
	offset = 0;
}

bool InputReg::IsValid(uint16_t index, uint16_t reg)
{
	if (index==this->offset)
		return true;
	return false;
}
bool InputReg::Write(uint16_t index, uint16_t regp)
{
	SInputReg* sregp = (SInputReg*)&regp;

	uint16_t reg = slave->getHolding(index);
	SInputReg* pinputreg = (SInputReg*)&reg;
	pinputreg->PinNumber = pin->GetPinNumber();
	pinputreg->Latch = sregp->Latch;
	pinputreg->LatchDir = sregp->LatchDir;
	slave->setHolding(offset,*((uint16_t*)pinputreg));
	return true;
}

void InputReg::SetOffset(uint16_t offset)
{
	this->offset = offset;
	uint16_t reg = slave->getHolding(offset);
	SInputReg* pinputreg = (SInputReg*)&reg;

	pinputreg->Latch = 0;
	pinputreg->Quality = 1;
	pinputreg->PinNumber = pin->GetPinNumber();
	slave->setHolding(offset,*((uint16_t*)pinputreg));
}

bool InputReg::Refresh()
{
	uint16_t reg = slave->getHolding(offset);
	SInputReg* pinputreg = (SInputReg*)&reg;
	if (pinputreg->Value!=pin->IsSet())
	{
		if (pinputreg->LatchDir == pin->IsSet())
			pinputreg->Latch = 1;

		pinputreg->Value = pin->IsSet();
		slave->setHolding(offset,*((uint16_t*)pinputreg));
		return true;
	}
	return false;
}

EDeviceType InputReg::GetDeviceType()
{
	return EDeviceType::EDeviceTypeBinInputs;
}

EModbusFunctions InputReg::GetModbusFunc()
{
	return EModbusFunctions::EModbusReadHoldingRegisters;
}

uint16_t InputReg::GetSizeInWords()
{
	return sizeof(SInputReg) / 2;
}


#endif
