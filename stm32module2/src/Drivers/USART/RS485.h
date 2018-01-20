#ifndef RS485_H
#define RS485_H

#include "USARTBase.h"
#include "IOPin.h"
#include "Queue.h"
#include "IUSARTHandler.h"
#include "IUSART.h"

class RS485 : IUSARTHandler
{
	IUSART* usart;
	IOPin* de;
	IOPin* re;
	Queue<char> queue;
	int timeOutUs;
	bool packetCompleted;
	void USARTReceivedData(char pdata);
	bool hasNewData;
	//IOPin* ledpin;
	bool recEnable;
	static RS485* instance;
public:
	void ReceiverTimeout();
	void ClearQueue();
	void OnReceiveData(char pdata);
	RS485(IUSART* usart,/*ITimer* ptimer,*/ IOPin* pde, IOPin* pre, int timeOutUs);
	bool PacketCompleted();

	void OnData(char data);
	void Send(char* pchar,char len);
	//void Send(CString & text);
	void Send(char pchar);
	bool Read(char & pchar);
	//void ClearFlags();
	//enable receiving data
	void RecEnable(bool enable);
};


#endif
