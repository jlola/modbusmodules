#ifndef RS485_H
#define RS485_H

#include "USARTBase.h"
#include "IOPin.h"
#include "IUSARTHandler.h"
#include "IUSART.h"
#include "ITimer.h"
#include "ITimerHandler.h"

typedef enum
{
	Receiving,
	Sending,
	Processing
} RS485States;

class RS485 : IUSARTHandler
{
	IUSART* usart;
	IOPin* de;
	IOPin* re;
	int timeOutUs;
	bool packetCompleted;
	char* sendBuffer;
	char sendBufferLength;
	bool sending;
	bool receiving;
	void USARTReceivedData(char pdata);
	uint16_t bufferDataLen;

	static RS485* instance;
	//RS485States state;

	uint16_t Rand(int bytesToSend);
public:
	void EndReceiving();
	void ReceiverTimeout();
	void SendingCompleted();
	void ReadBuffer(uint8_t* & buffer,uint16_t & size);
	void OnReceiveData(char pdata);
	RS485(IUSART* usart,/*ITimer* ptimer,*/ IOPin* pde, IOPin* pre, int timeOutUs);
	bool PacketCompleted();
	void OnReceiveData(uint8_t* data, uint16_t size, bool completed);
	void OnData(char data);
	void Send(char* pchar,char len);
	void CopyToBuffer(uint8_t* recdata,uint16_t size);
	void RecEnable(bool enable);
	void ResetBuffer();
	bool IsBusy();
};


#endif
