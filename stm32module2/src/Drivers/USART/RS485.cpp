
#ifndef UNITTEST

#include "RS485.h"
#include "USARTBase.h"
#include "Settings.h"

RS485::RS485(IUSART* usart, IOPin* pde, IOPin* pre, int ptimeOutUs)
: packetCompleted(false),queue(BUFFER_SIZE),recEnable(false)
{
	timeOutUs = ptimeOutUs;

	de = pde;
	re = pre;
	this->usart = usart;
	this->usart->SetTimeOut(4);
	this->usart->SetHandler(this);
	this->usart->HWControlledDE(true);
}



void RS485::ReceiverTimeout()
{
	usart->EnableTimeout(false);
	recEnable = false;
	packetCompleted = true;
}

void RS485::ClearQueue()
{
	while(queue.ElemNum())
		queue.Dequeue();
}

void RS485::OnReceiveData(char pdata)
{
	if (!recEnable) return;

	queue.Enqueue(pdata);
	usart->EnableTimeout(true);
}

void RS485::RecEnable(bool enable)
{
	recEnable = enable;
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

bool RS485::Read(char & pchar)
{
	if (queue.ElemNum()>0)
	{
		pchar = queue.Dequeue();
		return true;
	}
	return false;
}

void RS485::Send(char pchar)
{
	if (de!=NULL) de->Set(true);
	if (re!=NULL) re->Set(true);
	usart->Send(pchar);
	if (re!=NULL) re->Set(false);
	if (de!=NULL) de->Set(false);
}

void RS485::Send(char* pchar,char len)
{
	if (de!=NULL) de->Set(true);
	if (re!=NULL) re->Set(true);
	for(char i=0;i<len;i++)
		usart->Send(pchar[i]);
	if (de!=NULL) de->Set(false);
	if (re!=NULL) re->Set(false);
}

#endif
