#ifndef TIMER_H
#define TIMER_H

#include "ITimerHandler.h"
#include "ITimer.h"

extern "C" void TIM6_IRQHandler();


class Timer6 : public ITimer
{
	friend void TIM6_IRQHandler();
	ITimerHandler* rec;
	bool started;
	uint16_t timeus;
	Timer6();
public:
	static Timer6* Instance();
	void SetReceiver(ITimerHandler* prec);
	void Stop();
	void Start(ITimerHandler* t);
	void Start();
	void Reset();
	void Init();
	bool IsStarted();
	void SetTimeUs(uint16_t timeus);
	virtual ~Timer6();
};



#endif
