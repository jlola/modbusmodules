#include "OutputReg.h"


OutputReg::OutputReg()
{

}

void OutputReg::Init(uint16_t offset,IOPin* pin,SlaveRtu* slave,uint8_t pinNumber)
{
	this->pinNumber = pinNumber;
	this->offset = offset;
	this->pin = pin;
	this->slave = slave;
	uint16_t reg = slave->getHolding(offset);
	SOutputReg* poutputreg = (SOutputReg*)&reg;
	poutputreg->PinNumber = pinNumber;
	slave->setHolding(offset,*((uint16_t*)poutputreg));
}

void OutputReg::Refresh()
{
	uint16_t reg = slave->getHolding(offset);
	SOutputReg* poutputreg = (SOutputReg*)&reg;
	if (poutputreg->Value!=pin->IsSet())
	{
		pin->Set(poutputreg->Value);
	}
}

bool OutputReg::IsValid(uint16_t index, uint16_t reg)
{
	if (index==this->offset)
		return true;
	return false;
}

bool OutputReg::Write(uint16_t index, uint16_t regp)
{
	SOutputReg* sregp = (SOutputReg*)&regp;

	uint16_t reg = slave->getHolding(index);
	SOutputReg* poutputreg = (SOutputReg*)&reg;
	poutputreg->PinNumber = pinNumber;
	poutputreg->Value = sregp->Value;
	slave->setHolding(offset,*((uint16_t*)poutputreg));

	return true;
}
