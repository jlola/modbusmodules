
#if /*defined(UNITTEST) ||*/ defined(STM32F030C8)

#include <stm32f0xx_tim.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>
#include "stm32f0xx.h"
#include <stddef.h>
#include <Timer6.h>

TIM_TypeDef* tim = TIM6;

Timer6::Timer6()
	: rec(NULL),started(false),timeus(0)
{
}

void Timer6::SetReceiver(ITimerHandler* prec)
{
	rec = prec;
}

Timer6* Timer6::Instance()
{
	static Timer6 instance;
	return &instance;
}


void Timer6::SetTimeUs(uint16_t Timeus)
{
	uint32_t period = (10 * Timeus) - 1;
	/* Set the Autoreload value */
	tim->ARR = period;
}

void Timer6::Init()
{
	started = false;
	//rec = NULL;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 2000 - 1; // 1 MHz down to 100 KHz (10 us)
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // Down to 1 MHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(tim, &TIM_TimeBaseStructure);

	TIM_ITConfig(tim, TIM_IT_Update, ENABLE);
	TIM_Cmd(tim, DISABLE);

	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = TIM6_IRQn;

	//nvicStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;;
	nvicStructure.NVIC_IRQChannelPriority = 2;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

void Timer6::Reset()
{
	TIM_SetCounter(tim,0);
}

void Timer6::Start(ITimerHandler* prec)
{
	rec = prec;
	/* TIM2 enable counter */
	Start();
}

void Timer6::Start()
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

void Timer6::Stop()
{
	if (started)
	{
		started = false;
		/* TIM2 disable counter */
		TIM_Cmd(tim, DISABLE);

		rec = NULL;
	}
}

bool Timer6::IsStarted()
{
	return started;
}


extern "C" void TIM6_IRQHandler()
{
    if (TIM_GetITStatus(tim, TIM_IT_Update) != RESET)
    {
    	ITimerHandler* handler = Timer6::Instance()->rec;
        TIM_ClearITPendingBit(tim, TIM_IT_Update);
        if (handler!=NULL)
        	handler->OnHWTimer(1);
    }

    TIM_ClearITPendingBit(tim, TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 );
}

Timer6::~Timer6()
{
}

#endif
