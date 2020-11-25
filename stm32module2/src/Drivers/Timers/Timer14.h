/*
 * Timer14.h
 *
 *  Created on: 26. 1. 2019
 *      Author: pepa
 */

#ifndef DRIVERS_TIMERS_TIMER14_H_
#define DRIVERS_TIMERS_TIMER14_H_

#include "ITimerHandler.h"
#include "ITimer.h"

extern "C" void TIM14_IRQHandler();

class Timer14  : public ITimer
{
		friend void TIM14_IRQHandler();
		ITimerHandler* rec;
		bool started;
		uint16_t timeus;
		Timer14();
	public:
		static TIM_TypeDef* tim;
		static Timer14* Instance();
		void SetReceiver(ITimerHandler* prec);
		void Stop();
		void Start(ITimerHandler* t);
		void Start();
		void Reset();
		void Init();
		bool IsStarted();
		void SetTimeUs(uint16_t timeus);
		virtual ~Timer14();
};

#endif /* DRIVERS_TIMERS_TIMER14_H_ */
