#ifndef UNITTEST

#include "stm32f0xx.h"
#include "stm32f0xx_misc.h"			 // I recommend you have a look at these in the ST firmware folder
#include "stm32f0xx_usart.h" // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "USARTBase.h"


USARTBase::USARTBase(USART_TypeDef* usart)
{
	this->usart = usart;
	handler = NULL;
	isinit = false;
	baudrate = 0;
	usart = NULL;
	isbusy = false;
}

USART_TypeDef* USARTBase::GetUsart()
{
	return usart;
}

void USARTBase::ResetBusy()
{
	isbusy = false;
	//ReceiveEnable(false);
	//DMA1_Channel3->CNDTR = 255;
	//ReceiveEnable(true);
}

void USARTBase::SetBusy()
{
	isbusy = true;
}

bool USARTBase::IsIdle()
{
	FlagStatus status = USART_GetFlagStatus(usart,USART_FLAG_IDLE);
	if (status==SET)
		return true;

	return false;
}

bool USARTBase::IsBusy()
{
	FlagStatus busy = USART_GetFlagStatus(usart,USART_FLAG_BUSY);
	if (busy==SET)
	{
		isbusy = true;
	}
	return isbusy;//DMA1_Channel3->CNDTR < 255;



	//return false;
}

bool USARTBase::IsInitialized()
{
	return isinit;
}

uint32_t USARTBase::GetSpeed()
{
	return baudrate;
}

void USARTBase::SetHandler(IUSARTHandler* handler)
{
	this->handler = handler;
}

 IUSARTHandler* USARTBase::GetHandler()
{
	return handler;
}

 void USARTBase::HWControlledDE(bool enable)
 {
 	if (enable)
 	{
 		USART_Cmd(usart,DISABLE);
 		USART_DEPolarityConfig(usart,USART_DEPolarity_High);
 		USART_SetDEAssertionTime(usart,0x0F);
 		USART_SetDEDeassertionTime(usart,0x0F);
 		USART_DECmd(usart, ENABLE);
 		USART_Cmd(usart,ENABLE);
 	}
 }

void USARTBase::Send(char pchar)
{

	USART_SendData(usart, pchar);

	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
}

void USARTBase::Send(const char* pchar,size_t len)
{
	for(size_t i = 0; i<len; i++)
	{
		Send(pchar[i]);
	}
}

USARTBase::~USARTBase(){
}

#endif
