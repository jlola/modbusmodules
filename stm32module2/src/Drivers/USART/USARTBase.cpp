#include "stm32f0xx.h"
#include "stm32f0xx_misc.h"			 // I recommend you have a look at these in the ST firmware folder
#include "stm32f0xx_usart.h" // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "USARTBase.h"


USARTBase::USARTBase()
{
	isinit = false;
	baudrate = 0;
}

USART_TypeDef* USARTBase::GetUsart()
{
	return usart;
}

bool USARTBase::IsInitialized()
{
	return isinit;
}

uint32_t USARTBase::GetSpeed()
{
	return baudrate;
}

//void USARTBase::Send(CString& text)
//{
//
//	for(size_t i=0;i<text.length();i++)
//	{
//		USART_SendData(usart, text[i]);
//
//		while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
//		USART_ClearFlag(usart,USART_FLAG_TC);
//
//	}
//}

void USARTBase::Send(char pchar)
{

	USART_SendData(usart, pchar);

	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
}

void USARTBase::Send(char* pchar,size_t start,size_t len)
{
	for(size_t i = start;i<len;i++)
	{
		Send(pchar[i]);
	}
}


