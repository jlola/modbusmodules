#include "InputReg.h"

InputReg::InputReg()
{
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
	pinputreg->PinNumber = pinNumber;
	pinputreg->Latch = sregp->Latch;
	pinputreg->LatchDir = sregp->LatchDir;
	slave->setHolding(offset,*((uint16_t*)pinputreg));
	return true;
}

void InputReg::Init(uint16_t offset,IOPin* pin,SlaveRtu* slave,uint8_t pinNumber)
{
	this->pinNumber = pinNumber;
	this->offset = offset;
	this->pin = pin;
	this->slave = slave;
	uint16_t reg = slave->getHolding(offset);
	SInputReg* pinputreg = (SInputReg*)&reg;

	pinputreg->Latch = 0;
	pinputreg->Quality = 1;
	pinputreg->PinNumber = pinNumber;
	slave->setHolding(offset,*((uint16_t*)pinputreg));
}

void InputReg::Refresh()
{
	uint16_t reg = slave->getHolding(offset);
	SInputReg* pinputreg = (SInputReg*)&reg;
	if (pinputreg->Value!=pin->IsSet())
	{
		if (pinputreg->LatchDir == pin->IsSet())
			pinputreg->Latch = 1;

		pinputreg->Value = pin->IsSet();
		slave->setHolding(offset,*((uint16_t*)pinputreg));
	}
}



