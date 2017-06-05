#ifndef RS485_H
#define RS485_H

#include "USARTBase.h"
#include "IOPin.h"
#include "Timer.h"
#include "Queue.h"


class RS485
{

	USARTBase* usart;
	IOPin* de;
	IOPin* re;
	Queue<char> queue;
	//ITimer* timer;
	int timeOutUs;
	bool packetCompleted;
	void USARTReceivedData(char pdata);
	bool hasNewData;
	IOPin* ledpin;
	bool recEnable;

public:
	void ReceiverTimeout();
	void ClearQueue();
	void OnReceiveData(char pdata);
	RS485(USARTBase* pusart,/*ITimer* ptimer,*/ IOPin* pde, IOPin* pre, int timeOutUs);
	bool PacketCompleted();
	void OnHWTimer(int us);
	void Send(char* pchar,char len);
	//void Send(CString & text);
	void Send(char pchar);
	bool Read(char & pchar);
	void ClearFlags();
	//enable receiving data
	void RecEnable(bool enable);
};

void RS485SetInstance(RS485* Instance);


#endif
