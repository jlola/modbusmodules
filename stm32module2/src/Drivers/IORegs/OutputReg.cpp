#ifndef UNITTEST

#include "OutputReg.h"


OutputReg::OutputReg(IOPin* pin,IModbusSlave* slave)
{
	offset = 0;
	assert_param(slave != NULL);
	assert_param(pin != NULL);

	this->offset = 0;
	this->pin = pin;
	this->slave = slave;
}

void OutputReg::SetOffset(uint16_t offset)
{
	this->offset = offset;
	assert_param(offset>0);

	uint16_t reg = slave->getHolding(offset);
	SOutputReg* poutputreg = (SOutputReg*)&reg;
	poutputreg->PinNumber = pin->GetPinNumber();
	slave->setHolding(offset,*((uint16_t*)poutputreg));
}

void OutputReg::Refresh()
{
	assert_param(offset>0);

	uint16_t reg = slave->getHolding(offset);
	SOutputReg* poutputreg = (SOutputReg*)&reg;
	if (poutputreg->Value!=pin->IsSet())
	{
		pin->Set(poutputreg->Value);
	}
}

bool OutputReg::IsValid(uint16_t index, uint16_t reg)
{
	assert_param(offset>0);
	if (index==this->offset)
		return true;
	return false;
}

bool OutputReg::Write(uint16_t index, uint16_t regp)
{
	assert_param(offset>0);
	SOutputReg* sregp = (SOutputReg*)&regp;

	uint16_t reg = slave->getHolding(index);
	SOutputReg* poutputreg = (SOutputReg*)&reg;
	poutputreg->PinNumber = pin->GetPinNumber();
	poutputreg->Value = sregp->Value;
	slave->setHolding(offset,*((uint16_t*)poutputreg));

	return true;
}

EDeviceType OutputReg::GetDeviceType()
{
	return EDeviceType::EDeviceTypeBinOutputs;
}

EModbusFunctions OutputReg::GetModbusFunc()
{
	return EModbusFunctions::EModbusReadHoldingRegisters;
}

uint16_t OutputReg::GetSizeInWords()
{
	return sizeof(SOutputReg) / 2;
}


OutputReg::~OutputReg()
{

}

#endif
