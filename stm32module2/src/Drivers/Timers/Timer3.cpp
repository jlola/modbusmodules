#ifndef UNITTEST

#include <stm32f0xx_tim.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>
#include <stddef.h>
#include "Timer3.h"

Timer3::Timer3()
{
	tim = TIM3;
	started = false;
	Timer3::rec = NULL;
	SetTimeUs(10);
	periodus = 10;
}

void Timer3::Reset()
{
	TIM_SetCounter(tim,0);
}

void Timer3::SetReceiver(ITimerHandler* prec)
{
	this->rec = prec;
}

void Timer3::SetTimeUs(uint16_t Timeus)
{
	periodus = Timeus - 1;
	/* Set the Autoreload value */
	TIM3->ARR = periodus;
}

Timer3* Timer3::Instance()
{
	static Timer3 instance;
	return &instance;
}

void Timer3::Init()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 5 - 1; // 1 MHz down to 100 KHz (10 us)
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // Down to 1 MHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_ITConfig(tim, TIM_IT_Update, ENABLE);
	TIM_Cmd(tim, DISABLE);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM3_IRQn;

	//nvicStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;;
	nvicStructure.NVIC_IRQChannelPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

void Timer3::Start(ITimerHandler* prec)
{
	Timer3::rec = prec;
	/* TIM2 enable counter */
	Start();
}

void Timer3::Start()
{
	if (!started)
	{
		started = true;
		/* TIM2 enable counter */
		TIM_Cmd(tim, ENABLE);
	}
}

void Timer3::Stop()
{
	if (started)
	{
		started = false;
		/* TIM2 disable counter */
		TIM_Cmd(tim, DISABLE);

		Timer3::rec = NULL;
	}
}

bool Timer3::IsStarted()
{
	return started;
}


extern "C" void TIM3_IRQHandler()
{
	Timer3* inst = Timer3::Instance();
    if (TIM_GetITStatus(inst->tim, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(inst->tim, TIM_IT_Update);
        if (inst->rec!=NULL)
        	inst->rec->OnHWTimer(5);
    }

    TIM_ClearITPendingBit(TIM3, TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 );


}

#endif

