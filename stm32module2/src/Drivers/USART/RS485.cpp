
#ifndef UNITTEST

#include "RS485.h"
#include "USARTBase.h"
#include "Settings.h"
#include "string.h"
#include "stdlib.h"
#include "diag/Trace.h"

uint8_t _buffer[BUFFER_SIZE];

RS485::RS485(IUSART* usart, IOPin* de, IOPin* re, int timeOutUs)
: packetCompleted(false)
{
	this->timeOutUs = timeOutUs;
	this->sendBuffer = NULL;
	this->sendBufferLength = 0;
	this->de = de;
	this->re = re;
	this->usart = usart;
	this->usart->SetTimeOut(10);
	this->usart->SetHandler(this);
	this->usart->HWControlledDE(true);
	//this->state = Receiving;
	this->bufferDataLen = 0;
	usart->Enable(true);
	usart->ReceiveEnable(true);
	sending = false;
	receiving = false;
}


//from BUS IDLE - raised after send and receive
void RS485::OnReceiveData(uint8_t* data, uint16_t size, bool completed)
{
	if (sending)
	{
		sending = false;
		RecEnable(true);
		usart->ReceiveEnable(true);
		return;
	}

	CopyToBuffer(data,size);
	if (completed)
	{
		packetCompleted = true;
		receiving = true;
	}
}

void RS485::EndReceiving()
{
	receiving = false;
}


bool RS485::IsBusy()
{
	return usart->IsBusy() && sending && receiving;
}

void RS485::ReceiverTimeout()
{
	usart->EnableTimeout(false);
	packetCompleted = true;
}

//called from send dma interrupt
void RS485::SendingCompleted()
{
}

void RS485::OnReceiveData(char pdata)
{
}

void RS485::RecEnable(bool enable)
{
	if (enable)
	{
		usart->ReceiveEnable(true);
		if (re!=NULL) re->Set(false);
	}
	else
	{
		if (re!=NULL) re->Set(true);
	}
}

void RS485::ResetBuffer()
{
	bufferDataLen = 0;
}

bool RS485::PacketCompleted()
{
	if (packetCompleted)
	{
		packetCompleted = false;
		return true;
	}
	return false;
}

void RS485::CopyToBuffer(uint8_t* recdata,uint16_t size)
{
	bufferDataLen = size;
	memcpy(_buffer,recdata,size);
}

void RS485::ReadBuffer(uint8_t* & buffer,uint16_t & size)
{
	buffer = _buffer;
	size = bufferDataLen;
	ResetBuffer();
}



void RS485::Send(char* pchar,char len)
{
	sending = true;

	sendBuffer = pchar;
	sendBufferLength = len;

//	if (de!=NULL) de->Set(true);
//	if (re!=NULL) re->Set(true);
//	for(char i=0;i<len;i++)
//		usart->Send(pchar[i]);
//
//	if (de!=NULL) de->Set(false);
//	if (re!=NULL) re->Set(false);

	//if (de!=NULL) de->Set(true);
	//if (re!=NULL) re->Set(true);

	RecEnable(false);
	usart->ReceiveEnable(false);
	usart->Send(sendBuffer,sendBufferLength);
}

#endif
