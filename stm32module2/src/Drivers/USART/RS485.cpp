
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
	this->state = Receiving;
	this->bufferDataLen = 0;
}

void RS485::OnHWTimer(uint8_t us)
{
	timer->Stop();
	//if (!usart->IsBusy())
	//usart->Send(sendBuffer,sendBufferLength);

}

void RS485::OnReceiveData(uint8_t* data, uint16_t size, bool completed)
{
	trace_printf("received: %d\n",size);
	switch(state)
	{
		case Receiving:
			//trace_puts("OnReceiveData Receiving");

			CopyToBuffer(data,size);
			if (completed)
			{
				packetCompleted = true;
				state = Processing;
			}
			break;
		case Sending:
			trace_puts("OnReceiveData Sending");
			if (completed)
			{
				state = Receiving;
			}
			break;
		case Processing:
			trace_puts("OnReceiveData Processing");
			usart->SetBusy();
			if (completed)
				state = Receiving;
			break;
	}
}

void RS485::ReceiverTimeout()
{
	usart->EnableTimeout(false);
	packetCompleted = true;
}

void RS485::OnReceiveData(char pdata)
{
}

void RS485::RecEnable(bool enable)
{
	state = Receiving;
	usart->Enable(enable);
	usart->ReceiveEnable(enable);
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
}

uint16_t RS485::Rand()
{
	return ((rand() % 0xF)*10);
}

void RS485::Send(char* pchar,char len,bool withDelay)
{
	sendBuffer = pchar;
	sendBufferLength = len;

	state = Sending;

	uint16_t delay = this->Rand();
	if (withDelay)
	{
		timer->SetTimeUs(delay);
		timer->Start();
		while(timer->IsStarted());
	}

	if (!usart->IsBusy())
	{
		usart->Send(pchar,len);
		trace_printf("CNDTR: %d, sended: %d\n",DMA1_Channel3->CNDTR,len);
	}
	else
	{
		trace_printf("IsBusy, Delay: %d\n",delay);
		state = Receiving;
	}
	usart->ResetBusy();
}

#endif
