#include <stm32f0xx_tim.h>
#include <stm32f0xx_rcc.h>
#include <stm32f0xx_misc.h>
#include <stddef.h>
#include "Timer.h"


bool started;
uint16_t timeus;
//TimerTimer()
//{
//}

//void TimerSetReceiver(ITimerHandler* prec)
//{
//	rec = prec;
//}

//Timer* TimerInstance()
//{
//	static Timer instance;
//	return &instance;
//}


void TimerSetTimeUs(uint16_t Timeus)
{
	uint32_t period = (10 * Timeus) - 1;
	/* Set the Autoreload value */
	TIM2->ARR = period;
}

void TimerInit()
{
	started = false;
	//rec = NULL;

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	/* TIM2 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 10 - 1; // 1 MHz down to 100 KHz (10 us)
	TIM_TimeBaseStructure.TIM_Prescaler = 48 - 1; // Down to 1 MHz (adjust per your clock)
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, DISABLE);

	NVIC_InitTypeDef nvicStructure;
	//nvicStructure.NVIC_IRQChannel = TIM2_IRQn;

	//nvicStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;;
	nvicStructure.NVIC_IRQChannelPriority = 2;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

//void TimerStart(ITimerHandler* prec)
//{
//	//rec = prec;
//	/* TIM2 enable counter */
//	Start();
//}

void TimerStart()
{
	if (!started)
	{
		started = true;
		/* TIM2 enable counter */
		TIM_Cmd(TIM2, ENABLE);
	}
	TIM2->CNT = 0x0000;
}

void TimerStop()
{
	if (started)
	{
		started = false;
		/* TIM2 disable counter */
		TIM_Cmd(TIM2, DISABLE);

		//rec = NULL;
	}
}

bool TimerIsStarted()
{
	return started;
}


//extern "C" void TIM2_IRQHandler()
//{
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//    {
//        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//        if (Timer::Instance()->rec!=NULL)
//        	Timer::Instance()->rec->OnHWTimer(1);
//    }
//
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Trigger | TIM_IT_Break | TIM_IT_COM | TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 );
//
//
//}
