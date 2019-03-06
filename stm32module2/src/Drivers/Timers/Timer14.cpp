/*
 * Timer14.cpp
 *
 *  Created on: 26. 1. 2019
 *      Author: pepa
 */

#include <stm32f0xx_tim.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>
#include "stm32f0xx.h"
#include <stddef.h>
#include <Timer14.h>

TIM_TypeDef* tim = TIM14;

Timer14::Timer14()
	: rec(NULL),started(false),timeus(0)
{
}

void Timer14::SetReceiver(ITimerHandler* rec)
{
	this->rec = rec;
}

Timer14* Timer14::Instance()
{
	static Timer14 instance;
	return &instance;
}


void Timer14::SetTimeUs(uint16_t Timeus)
{
	uint32_t period = (Timeus) - 1;
	/* Set the Autoreload value */
	tim->ARR = period;
}

void Timer14::Init()
{
	started = false;
	//rec = NULL;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 1; // 1 MHz down to 100 KHz (10 us)
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // Down to 1 MHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);

	TIM_ITConfig(tim, TIM_IT_Update, ENABLE);
	TIM_Cmd(tim, DISABLE);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM14_IRQn;

	//nvicStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;;
	nvicStructure.NVIC_IRQChannelPriority = 2;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

void Timer14::Reset()
{
	TIM_SetCounter(tim,0);
}

void Timer14::Start(ITimerHandler* prec)
{
	rec = prec;
	/* TIM2 enable counter */
	Start();
}

void Timer14::Start()
{
	assert_param(rec);

	if (!started)
	{
		started = true;
		/* TIM2 enable counter */
		TIM_Cmd(tim, ENABLE);
	}
	tim->CNT = 0x0000;
}

void Timer14::Stop()
{
	if (started)
	{
		started = false;
		/* TIM2 disable counter */
		TIM_Cmd(tim, DISABLE);

		//rec = NULL;
	}
}

bool Timer14::IsStarted()
{
	return started;
}


extern "C" void TIM14_IRQHandler()
{
    if (TIM_GetITStatus(tim, TIM_IT_Update) != RESET)
    {
    	ITimerHandler* handler = Timer14::Instance()->rec;
        TIM_ClearITPendingBit(tim, TIM_IT_Update);
        if (handler!=NULL)
        	handler->OnHWTimer(1);
    }

    TIM_ClearITPendingBit(tim, TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 );
}

Timer14::~Timer14()
{
}

