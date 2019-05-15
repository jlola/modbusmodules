#ifndef UNITTEST

#include <stdlib.h>
#include <USART1.h>
#include "stm32f0xx.h"
//#include "misc.h"			 // I recommend you have a look at these in the ST firmware folder
#include "stm32f0xx_usart.h" // under Libraries/STM32F4xx_StdPeriph_Driver/inc and src
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_gpio.h"
#include "string.h"

uint8_t RxBuffer[512];
uint8_t TxBuffer[255];

CUSART1::CUSART1()
	: USARTBase::USARTBase(USART1)
{
	memset(RxBuffer,0x00,sizeof(RxBuffer));
}

void CUSART1::Enable(bool enable)
{
	if (enable)
		USART_Cmd(usart, ENABLE);
	else
		USART_Cmd(usart, DISABLE);
}

void CUSART1::SetTimeOut(uint8_t bits)
{
	USART_SetReceiverTimeOut(usart,bits);
}

void CUSART1::Send(const char* pchar,size_t len)
{
	len = len > sizeof(TxBuffer) ? sizeof(TxBuffer) : len;
	memcpy(TxBuffer,pchar,len);
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA1_Channel2->CNDTR = len;
	/* Enable USARTy TX DMA1 Channel */
	DMA_Cmd(DMA1_Channel2, ENABLE);
}

void CUSART1::EnableTimeout(bool enable)
{
	if (enable)
		USART_ReceiverTimeOutCmd(usart,ENABLE);
	else
		USART_ReceiverTimeOutCmd(usart,DISABLE);
}

void CUSART1::Init(uint32_t baudrate)
{
	if (isinit) return;
	isinit = true;

	InitUsart(baudrate);
	DMA_ConfigurationUsartReceive();
	//DMA_ConfigurationUsartSend();
}

void CUSART1::InitUsart(uint32_t pbaudrate)
{
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
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

		/* enable the peripheral clock for the pins used by
		 * USART1, PB6 for TX and PB7 for RX
		 */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);


		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);



		/* This sequence sets up the TX and RX pins
		 * so they work correctly with the USART1 peripheral
		 */
#ifdef STM32F030C8
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12; // Pins 9 (TX) and 10 (RX) are used
#elif defined STM32F030K6
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1; // Pins 2 (TX) and 3 (RX) are used
#else
	#error "Not selected cpu. Please define STM32F030 or STM32F030XC"
#endif
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOA, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

		/* The RX and TX pins are now connected to their AF
		 * so that the USART2 can take over control of the
		 * pins
		 */
#ifdef STM32F030C8
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_1);
#elif defined STM32F030K6
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); //
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1);
#else
	#error "Not selected cpu. Please define STM32F030 or STM32F030XC"
#endif

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
		USART_ITConfig(usart, USART_IT_IDLE, ENABLE);
		USART_ITConfig(usart, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt

		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;// this sets the priority group of the USART1 interrupts
		//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
		NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

		// finally this enables the complete USART1 peripheral
		//moved to Enable method
		USART_Cmd(usart, ENABLE);
}

void CUSART1::DMA_ConfigurationUsartSend()
{
	DMA_InitTypeDef DMA_InitStructure;
	//NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)
	DMA_Channel_TypeDef* dma1ChannelSend = DMA1_Channel2;

	DMA_DeInit(dma1ChannelSend);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&usart->TDR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)TxBuffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = sizeof(TxBuffer);
	DMA_Init(dma1ChannelSend, &DMA_InitStructure);

	/* Enable USARTy DMA Rx and TX request */
	USART_DMACmd(usart, USART_DMAReq_Tx, ENABLE);

	/* Enable USARTy TX DMA1 Channel */
	//DMA_Cmd(dma1ChannelSend, ENABLE);
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC ,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;// this sets the priority group of the USART1 interrupts
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);
}

void CUSART1::DMA_ConfigurationUsartReceive()
{
  DMA_InitTypeDef DMA_InitStructure;

  DMA_DeInit(DMA1_Channel3);
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//DMA_Mode_Circular
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&usart->RDR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = sizeof(RxBuffer);
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);

  /* Enable USARTy DMA Rx and TX request */
   USART_DMACmd(usart, USART_DMAReq_Rx ,/*| USART_DMAReq_Tx,*/ ENABLE);

    /* Enable USARTy RX DMA1 Channel */
   DMA_Cmd(DMA1_Channel3, ENABLE);

//  NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)
//  DMA_ITConfig(DMA1_Channel3,DMA_IT_TC ,ENABLE);
//	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;		 // we want to configure the USART1 interrupts
//	NVIC_InitStructure.NVIC_IRQChannelPriority = 2;// this sets the priority group of the USART1 interrupts
//	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART interrupts are globally enabled
//	NVIC_Init(&NVIC_InitStructure);
}

void CUSART1::ReceiveEnable(bool enable)
{
	if (enable)
	{
		//DMA_Cmd(DMA1_Channel3, DISABLE);
		DMA1_Channel3->CNDTR = sizeof(RxBuffer);
		DMA_Cmd(DMA1_Channel3, ENABLE);
	}
	else
		DMA_Cmd(DMA1_Channel3, DISABLE);
}

extern "C" void DMA1_Channel2_3_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_IT_TC2)==SET)
	{
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_Cmd(DMA1_Channel2, DISABLE);
		IUSARTHandler* handler = CUSART1::Instance()->GetHandler();
		if (handler!=NULL)
			handler->SendingCompleted();
	}
}

/**
 * \brief           Check for new data received with DMA
 * \note            This function must be called from DMA HT/TC and USART IDLE events
 * \note            Full source code is available in examples
 */
void
usart_rx_check(size_t pos) {
    static size_t old_pos;
    IUSARTHandler* handler = CUSART1::Instance()->GetHandler();
    /* Calculate current position in buffer */
    //pos = ARRAY_LEN(usart_rx_dma_buffer) - LL_DMA_GetDataLength(DMA1, LL_DMA_STREAM_1);
    if (pos != old_pos) {                       /* Check change in received data */
        if (pos > old_pos) {                    /* Current position is over previous one */
            /* We are in "linear" mode, case P1, P2, P3 */
            /* Process data directly by subtracting "pointers" */
        	if (handler!=NULL)
        		handler->OnReceiveData(&RxBuffer[old_pos], pos - old_pos,true);
        } else {
            /* We are in "overflow" mode, case P4 */
            /* First process data to the end of buffer */
        	if (handler!=NULL)
        	{
        	    handler->OnReceiveData(&RxBuffer[old_pos], sizeof(RxBuffer) - old_pos,false);
            /* Continue with beginning of buffer */
        	    handler->OnReceiveData(&RxBuffer[0], pos,true);
        	}
        }
    }
    old_pos = pos;                              /* Save current position as old */

    /* Check and manually update if we reached end of buffer */
    if (old_pos == sizeof(RxBuffer)) {
        old_pos = 0;
    }
}

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
extern "C" void USART1_IRQHandler(void)
{
	IUSARTHandler* handler = CUSART1::Instance()->GetHandler();
	USART_TypeDef* usart = CUSART1::Instance()->GetUsart();
	// check if the USART1 receive interrupt flag was set
	if (USART_GetITStatus(usart,USART_IT_RTO))
	{

		USART_ClearITPendingBit(usart,USART_IT_RTO);
		if (handler!=NULL)
			handler->ReceiverTimeout();

	}
	else if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		CUSART1::Instance()->SetBusy();
		USART_ReceiveData(usart);
	}
	else if(USART_GetITStatus(usart,USART_IT_ORE)||USART_GetFlagStatus(usart,USART_FLAG_ORE))
	{
		USART_ReceiveData(usart);
		//instance->ClearQueue();
		USART_ClearITPendingBit(usart,USART_IT_ORE);
	}
	else if(USART_GetITStatus(usart,USART_IT_IDLE))
	{
		USART_ClearITPendingBit(usart,USART_IT_IDLE);
		DMA_Cmd(DMA1_Channel3, DISABLE);
		auto cndtr = sizeof(RxBuffer) - DMA1_Channel3->CNDTR;
		//DMA1_Channel3->CNDTR = sizeof(RxBuffer);
		if (handler!=NULL)
			handler->OnReceiveData(RxBuffer,cndtr,true);
		//DMA_Cmd(DMA1_Channel3, ENABLE);
		//usart_rx_check(cndtr);
	}
	else if(USART_GetITStatus(usart,USART_IT_NE ))
	{
		USART_ClearITPendingBit(usart,USART_IT_NE );
	}
}



CUSART1* CUSART1::Instance()
{
	static CUSART1 instance;
	return &instance;
}

#endif
