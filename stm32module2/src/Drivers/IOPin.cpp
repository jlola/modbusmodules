#ifndef UNITTEST

#include "IOPin.h"

IOPin::IOPin(GPIO_TypeDef* pGPIOx,uint16_t pGPIO_Pin,IODirection pdir,uint8_t pinNumber)
{
	this->pinNumber = pinNumber;
	GPIOx = pGPIOx;
	GPIO_Pin = pGPIO_Pin;
	dir = pdir;
	Init();
}

uint8_t IOPin::GetPinNumber()
{
	return this->pinNumber;
}

void IOPin::Init()
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	switch(dir)
	{
		case IOInput:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			break;
		case IOOutput:
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
			GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
			break;
	}

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void IOPin::Set(bool set)
{
	if (set)
		GPIO_SetBits(GPIOx,GPIO_Pin);
	else
		GPIO_ResetBits(GPIOx,GPIO_Pin);
}

bool IOPin::IsSet()
{
	switch(dir)
	{
		case IOInput:
			return GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==Bit_SET ? true : false;
		case IOOutput:
			return GPIO_ReadOutputDataBit(GPIOx,GPIO_Pin)==Bit_SET ? true : false;
	}
	return false;
}
#endif
