#ifndef TIMER3_H
#define TIMER3_H

#include "stdint.h"
#include "ITimerHandler.h"
#include "ITimer.h"

extern "C" void TIM3_IRQHandler();

class Timer3 : public ITimer
{
	TIM_TypeDef* tim;
	bool started;
	uint32_t periodus;
	ITimerHandler* rec;
public:
	Timer3();
	static Timer3* Instance();
	void Stop();
	void Start(ITimerHandler* t);
	void Start();
	void Reset();
	void Init();
	bool IsStarted();
	void SetTimeUs(uint16_t timeus);
	void SetReceiver(ITimerHandler* pkeg);
	friend void TIM3_IRQHandler();
};



#endif
