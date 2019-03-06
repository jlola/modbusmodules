/*
 * USART2.cpp
 *
 *  Created on: 19. 11. 2017
 *      Author: Libor
 */

#ifndef UNITTEST

#include <stdlib.h>
#include "stm32f0xx.h"
//#include "misc.h"			 // I recommend you have a look at these in the ST firmware folder
#include "stm32f0xx_usart.h" // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_dma.h"
#include <USART2.h>

//uint8_t RxBuffer[16];

CUSART2::CUSART2()
	: USARTBase::USARTBase(USART2)
{

}

void CUSART2::Enable(bool enable)
{
	if (enable)
		USART_Cmd(usart, ENABLE);
	else
		USART_Cmd(usart, DISABLE);
}

void CUSART2::SetTimeOut(uint8_t bits)
{
	USART_SetReceiverTimeOut(usart,bits);
}

void CUSART2::EnableTimeout(bool enable)
{
	if (enable)
		USART_ReceiverTimeOutCmd(usart,ENABLE);
	else
		USART_ReceiverTimeOutCmd(usart,DISABLE);
}

void CUSART2::Init(uint32_t pbaudrate)
{
	if (isinit) return;
	isinit = true;

	baudrate = pbaudrate;

	/* This is a concept that has to do with the libraries provided by ST
		 * to make development easier the have made up something similar to
		 * classes, called TypeDefs, which actually just define the common
		 * parameters that every peripheral needs to work correctly
		 *
		 * They make our life easier because we don't have to mess around with
		 * the low level stuff of setting bits in the correct registers
		 */
		GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
		USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization
		NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)

		/* enable APB2 peripheral clock for USART1
		 * note that only USART1 and USART6 are connected to APB2
		 * the other USARTs are connected to APB1
		 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		/* enable the peripheral clock for the pins used by
		 * USART1, PB6 for TX and PB7 for RX
		 */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_DMA1, ENABLE);

		/* This sequence sets up the TX and RX pins
		 * so they work correctly with the USART1 peripheral
		 */
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2; // Pins 2 (TX) and 3 (RX) are used
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOA, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

		/* The RX and TX pins are now connected to their AF
		 * so that the USART2 can take over control of the
		 * pins
		 */
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); //
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
		//GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_0);

		/* Now the USART_InitStruct is used to define the
		 * properties of USART1
		 */
		USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
		USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
		USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
		USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
		USART_Init(usart, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting


		/* Here the USART1 receive interrupt is enabled
		 * and the interrupt controller is configured
		 * to jump to the USART1_IRQHandler() function
		 * if the USART1 receive interrupt occurs
		 */
		USART_ITConfig(usart, USART_IT_RXNE, ENABLE); // enable the USART2 receive interrupt
		USART_ITConfig(usart, USART_IT_RTO, ENABLE); // enable the USART2 receive interrupt

		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 // we want to configure the USART1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelPriority = 1;// this sets the priority group of the USART1 interrupts
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART interrupts are globally enabled
		NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

		//DMA_Configuration();
		// finally this enables the complete USART peripheral
		USART_Cmd(usart, ENABLE);
}

void CUSART2::ReceiveEnable(bool enable)
{
	if (enable)
		USART_ITConfig(usart, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
	else
		USART_ITConfig(usart, USART_IT_RXNE, DISABLE); // enable the USART1 receive interrupt
}

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
extern "C" void USART2_IRQHandler(void)
{
	IUSARTHandler* handler = CUSART2::Instance()->GetHandler();
	USART_TypeDef* usart = CUSART2::Instance()->GetUsart();
	char pdata = 0x00;
	// check if the USART1 receive interrupt flag was set
	if (USART_GetITStatus(usart,USART_IT_RTO))
	{
		USART_ClearITPendingBit(usart, USART_IT_RTO);
		if (handler!=NULL)
			handler->ReceiverTimeout();
	}
	else if(USART_GetITStatus(usart,USART_IT_RXNE))
	{
		pdata = USART_ReceiveData(usart);
		if (handler!=NULL)
			handler->OnReceiveData(pdata);
	}
	else if(USART_GetITStatus(usart,USART_IT_ORE) || USART_GetFlagStatus(usart,USART_FLAG_ORE))
	{
		pdata = USART_ReceiveData(usart);
		//instance->ClearQueue();
		USART_ClearITPendingBit(usart,USART_IT_ORE);
	}
	else if(USART_GetITStatus(usart,USART_IT_IDLE))
	{
		USART_ClearITPendingBit(usart,USART_IT_IDLE);
	}
	else if(USART_GetITStatus(usart,USART_IT_NE ))
	{
		USART_ClearITPendingBit(usart,USART_IT_NE );
	}
}



USARTBase* CUSART2::Instance()
{
	static CUSART2 instance;
	return &instance;
}

CUSART2::~CUSART2()
{

}

#endif

