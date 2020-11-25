/*
 * RFIDRegs.cpp
 *
 *  Created on: 13. 11. 2017
 *      Author: Libor
 */

#include <string.h>
#include <RFID/RFIDRegs.h>
#include "diag/Trace.h"

RFIDRegs::RFIDRegs(IUSART *usart, IModbusSlave* pslave, ITimer* ptimer)
	: bufferIndex(0),offset(0),slave(pslave),timer(ptimer)
{
	memset(&regs,0x00,sizeof(regs));
	this->usart = usart;
	usart->SetHandler(this);
}

bool RFIDRegs::IsValid(uint16_t index, uint16_t value)
{
	if (index==this->offset)
		return true;
	return false;
}
bool RFIDRegs::Write(uint16_t index, uint16_t value)
{
	if (index==this->offset && value == 0)
	{
		regs.NewDataFlag = 0;
		return true;
	}
	return false;
}

void RFIDRegs::SendingCompleted()
{

}

void RFIDRegs::OnReceiveData(uint8_t* data, uint16_t size, bool completed)
{

}

void RFIDRegs::OnReceiveData(char data)
{
	if (!timer->IsStarted())
		timer->Start(this);
	timer->Reset();
	if (bufferIndex < sizeof(regs.buffer))
		regs.buffer[bufferIndex++] = data;
}

void RFIDRegs::OnHWTimer(uint8_t us)
{
	timer->Stop();
	ReceiverTimeout();
}

void RFIDRegs::ReceiverTimeout()
{
	regs.buffer[bufferIndex+1] = '\0';
	bufferIndex = 0;
	regs.NewDataFlag = 1;
	//trace_puts((const char*)regs.buffer);
}

bool RFIDRegs::Refresh()
{
	//uint16_t reg = slave->getHolding(offset);
	this->slave->setHoldings(offset,(uint16_t*)&regs,sizeof(regs)/2, true);
	if (regs.NewDataFlag)
		return true;
	return false;
}

EDeviceType RFIDRegs::GetDeviceType()
{
	return EDeviceTypeRFID;
}

EModbusFunctions RFIDRegs::GetModbusFunc()
{
	return EModbusReadHoldingRegisters;
}

uint16_t RFIDRegs::GetSizeInWords()
{
	return sizeof(SRFIDRegs) / 2;
}

void RFIDRegs::SetOffset(uint16_t offset)
{
	this->offset = offset;
}

RFIDRegs::~RFIDRegs()
{

}

