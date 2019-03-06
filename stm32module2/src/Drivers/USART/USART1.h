#ifndef CUSART2_H_
#define CUSART2_H_

#include <stdint.h>
#include "USARTBase.h"

class CUSART1 : public USARTBase
{
private:
	CUSART1();
	void DMA_ConfigurationUsartSend();
	void DMA_ConfigurationUsartReceive(void);
	void usart_rx_check(size_t pos);
public:
	void Send(const char* pchar,size_t len);
	void SetTimeOut(uint8_t bits);
	void EnableTimeout(bool enable);
	void Init(uint32_t speed);
	void Enable(bool enable);
	void ReceiveEnable(bool enable);
	void InitUsart(uint32_t pbaudrate);
	static CUSART1* Instance();
};



#endif
