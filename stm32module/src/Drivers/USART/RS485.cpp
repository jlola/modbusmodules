#include "RS485.h"
#include "USARTBase.h"
#include "ModbusSettings.h"
#include "stm32f0xx.h"
#include <stm32f0xx_tim.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>
#include "stm32f0xx_usart.h"

#define TIMER TIM2

RS485* instance = NULL;

void RS485SetInstance(RS485* Instance)
{
	instance = Instance;
}

extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIMER, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIMER, TIM_IT_Update);
        if (instance) instance->OnHWTimer(1);
    }

    TIM_ClearITPendingBit(TIMER, TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 );
}

void RS485::OnHWTimer(int us)
{
	TimerStop();
	packetCompleted = true;
}

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
extern "C" void USART1_IRQHandler(void){

	char pdata = 0x00;
	// check if the USART1 receive interrupt flag was set
	if (USART_GetITStatus(USART1,USART_IT_RTO))
	{
		USART_ClearITPendingBit(USART1,USART_IT_RTO);
		instance->ReceiverTimeout();
	}
	else if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		pdata = USART_ReceiveData(USART1);
		if (instance) instance->OnReceiveData(pdata);
	}
	else if(USART_GetITStatus(USART1,USART_IT_ORE)||USART_GetFlagStatus(USART1,USART_FLAG_ORE))
	{
		pdata = USART_ReceiveData(USART1);
		//instance->ClearQueue();
		USART_ClearITPendingBit(USART1,USART_IT_ORE);
	}
	else if(USART_GetITStatus(USART1,USART_IT_IDLE))
	{
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	}
	else if(USART_GetITStatus(USART1,USART_IT_NE ))
	{
		USART_ClearITPendingBit(USART1,USART_IT_NE );
	}
}

void RS485::ClearFlags()
{
	char pdata;
	packetCompleted = false;
	if (USART_GetFlagStatus(USART1,USART_FLAG_NE))
	{
		instance->ClearQueue();
		USART_ClearFlag(USART1,USART_FLAG_NE);
		pdata = USART1->RDR; // the character from the USART2 data register is saved in t
	}
	else if (USART_GetFlagStatus(USART1,USART_FLAG_FE))
	{
		instance->ClearQueue();
		USART_ClearFlag(USART1,USART_FLAG_FE);
		pdata = USART1->RDR; // the character from the USART2 data register is saved in t
	}
}

void RS485::ReceiverTimeout()
{
	usart->EnableTimeout(false);
	recEnable = false;
	packetCompleted = true;
}

void RS485::ClearQueue()
{
	while(queue.ElemNum())
		queue.Dequeue();
}

void RS485::OnReceiveData(char pdata)
{
	if (!recEnable) return;

	queue.Enqueue(pdata);

	#ifdef TIMER_CONTROL
	if (!TimerIsStarted())
		TimerStart();
	#else
	usart->EnableTimeout(true);
	#endif
}

RS485::RS485(USARTBase* pusart/*,ITimer* ptimer*/, IOPin* pde, IOPin* pre, int ptimeOutUs,IOPin* ledpin)
: packetCompleted(false),queue(BUFFER_SIZE),recEnable(false)
{
	this->ledpin = ledpin;
	timeOutUs = ptimeOutUs;
	#ifdef TIMER_CONTROL
	TimerSetTimeUs(timeOutUs);
	usart->EnableTimeout(false);
	#endif
	//timer = ptimer;

	de = pde;
	re = pre;
	usart = pusart;
	usart->SetTimeOut(4);
	//usart->Received = OnUSARTReceived::from_method<RS485,&RS485::USARTReceivedData>(this);
	if (!de)
	{
		USART_TypeDef* usarts = usart->GetUsart();
		USART_Cmd(usarts,DISABLE);
		USART_DEPolarityConfig(usarts,USART_DEPolarity_High);
		USART_SetDEAssertionTime(usarts,0x0F);
		USART_SetDEDeassertionTime(usarts,0x0F);
		USART_DECmd(usarts, ENABLE);
		USART_Cmd(usarts,ENABLE);
	}
}

void RS485::RecEnable(bool enable)
{
	recEnable = enable;
}

void RS485::ToogleLedPin()
{
	if (ledpin->IsSet())
		ledpin->Set(false);
	else
		ledpin->Set(true);
}

bool RS485::PacketCompleted()
{
	if (packetCompleted)
	{
		packetCompleted = false;
		return true;
	}
	return false;
}

bool RS485::Read(char & pchar)
{
	if (queue.ElemNum()>0)
	{
		pchar = queue.Dequeue();
		return true;
	}
	return false;
}

void RS485::Send(char pchar)
{
	if (de!=NULL) de->Set(true);
	if (re!=NULL) re->Set(true);
	usart->Send(pchar);
	if (re!=NULL) re->Set(false);
	if (de!=NULL) de->Set(false);
}

void RS485::Send(char* pchar,char len)
{
	if (de!=NULL) de->Set(true);
	if (re!=NULL) re->Set(true);
	for(char i=0;i<len;i++)
		usart->Send(pchar[i]);
	if (de!=NULL) de->Set(false);
	if (re!=NULL) re->Set(false);
}

//void RS485::Send(CString & text)
//{
//	if (de!=NULL) de->Set(true);
//	if (re!=NULL) re->Set(true);
//	usart->Send(text);
//	if (de!=NULL) de->Set(false);
//	if (re!=NULL) re->Set(false);
//}
