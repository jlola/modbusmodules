#ifndef USARTBASE_H_
#define USARTBASE_H_

#include <stm32f0xx.h>
#include <stm32f0xx_misc.h>			 // I recommend you have a look at these in the ST firmware folder
#include <stm32f0xx_usart.h> // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
//#include "event.h"
//#include "delegate.h"
#include "stdint.h"
#include <ctype.h>

//using namespace srutil;


//#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
//typedef srutil::delegate<void (char recchar)> OnUSARTReceived;
//#else
// typedef srutil::delegate<void, char> OnReceived;
//#endif

class USARTBase
{
protected:
	 bool isinit;
	USART_TypeDef* usart;
	USARTBase();
	uint32_t baudrate;
public:
	bool IsInitialized();
	USART_TypeDef* GetUsart();
	virtual void Init(uint32_t speed)=0;
	//virtual void Send(CString& text);
	virtual void Send(char pchar);
	virtual void Enable(bool enable)=0;
	//OnUSARTReceived Received;
	void Send(char* pchar,size_t start,size_t len);
	uint32_t GetSpeed();
	virtual void SetTimeOut(uint8_t bits)=0;
	virtual void EnableTimeout(bool enable)=0;
};

#endif
