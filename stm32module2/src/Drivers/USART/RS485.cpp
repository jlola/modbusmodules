
#ifndef UNITTEST

#include "RS485.h"
#include "USARTBase.h"
#include "Settings.h"
#include "string.h"
#include "stdlib.h"
#include "diag/Trace.h"

uint8_t _buffer[BUFFER_SIZE];

RS485::RS485(IUSART* usart, IOPin* de, IOPin* re, int timeOutUs, ITimer* timer)
: packetCompleted(false)
{
	this->timer = timer;
	this->timeOutUs = timeOutUs;
	this->sendBuffer = NULL;
	this->sendBufferLength = 0;
	this->de = de;
	this->re = re;
	this->usart = usart;
	this->usart->SetTimeOut(10);
	this->usart->SetHandler(this);
	this->usart->HWControlledDE(true);
	this->timer->SetReceiver(this);
	//this->state = Receiving;
	this->bufferDataLen = 0;
	usart->Enable(true);
	usart->ReceiveEnable(true);
}

void RS485::OnHWTimer(uint8_t us)
{
	timer->Stop();
}

void RS485::OnReceiveData(uint8_t* data, uint16_t size, bool completed)
{
	//trace_puts("OnReceiveData Receiving");
	//usart->ReceiveEnable(false);
	CopyToBuffer(data,size);
	if (completed)
	{
		//trace_printf("received: %d\n",bufferDataLen);
		packetCompleted = true;
		//state = Processing;
	}
}

void RS485::ReceiverTimeout()
{
	usart->EnableTimeout(false);
	packetCompleted = true;
}

//called from send dma interrupt
void RS485::SendingCompleted()
{
	RecEnable(true);
	//state = Receiving;
}

void RS485::OnReceiveData(char pdata)
{
}

void RS485::RecEnable(bool enable)
{
	if (enable)
	{
		usart->ReceiveEnable(true);
		//state = Receiving;
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
	bufferDataLen += size;
	memcpy(_buffer,recdata,size);
}

void RS485::ReadBuffer(uint8_t* & buffer,uint16_t & size)
{
	buffer = _buffer;
	size = bufferDataLen;
	ResetBuffer();
}

uint16_t RS485::Rand(int bytesToSend)
{
	uint16_t max = 0xFFFF;
	const int bits = 10;
	const int microsecondsPerBit = 9;
	const int result = bytesToSend*bits*microsecondsPerBit;
	uint64_t rnd =  (rand() % 0x3)*result;
	return rnd > max ? max : rnd;
}

void RS485::Send(char* pchar,char len,bool withDelay)
{
	sendBuffer = pchar;
	sendBufferLength = len;

	//state = Sending;
	if (withDelay)
	{
		//usart->IsBusy();
		uint16_t delay = this->Rand(len);
		if (delay>0)
		{
			timer->SetTimeUs(delay);
			timer->Start();
			while(timer->IsStarted());
		}
	}

	//if (!usart->IsBusy() || !withDelay)
//	{
		//usart->Send(pchar,len);
		if (de!=NULL) de->Set(true);
		if (re!=NULL) re->Set(true);
		for(char i=0;i<len;i++)
			usart->Send(pchar[i]);
		//state = Receiving;
		//ResetBuffer();
		if (de!=NULL) de->Set(false);
		if (re!=NULL) re->Set(false);
		//trace_printf("delay: %d, sended: %d\n",delay,len);
//	}
//	else
//	{
//		//trace_printf("IsBusy, Delay: %d\n",delay);
//		state = Receiving;
//	}
}

#endif
