#ifndef USARTBASE_H_
#define USARTBASE_H_

#include <stm32f0xx.h>
#include <stm32f0xx_misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f0xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
//#include "event.h"
//#include "delegate.h"
#include "stdint.h"
#include <ctype.h>
#include "IUSARTHandler.h"
#include "IUSART.h"

class USARTBase : public IUSART
{
protected:
	USART_TypeDef* usart;
	bool isinit;
	USARTBase(USART_TypeDef* usart);
	uint32_t baudrate;
	IUSARTHandler* handler;
	bool received;
public:
	void SetBusy();
	bool IsIdle();
	bool IsBusy();
	void ResetBusy();
	void HWControlledDE(bool enable);
	bool IsInitialized();
	USART_TypeDef* GetUsart();
	virtual void Init(uint32_t speed)=0;
	virtual void Send(char pchar);
	virtual void Enable(bool enable)=0;
	//OnUSARTReceived Received;
	virtual void Send(const char* pchar,size_t len);
	void SetHandler(IUSARTHandler* handler);
	IUSARTHandler* GetHandler();
	uint32_t GetSpeed();
	virtual void SetTimeOut(uint8_t bits)=0;
	virtual void EnableTimeout(bool enable)=0;
	virtual ~USARTBase();
};

#endif
