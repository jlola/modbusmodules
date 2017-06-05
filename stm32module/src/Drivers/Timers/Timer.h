#ifndef TIMER_H
#define TIMER_H

//#include "ITimerHandler.h"
//#include "ITimer.h"

//extern "C" void TIM2_IRQHandler();


//class Timer : public ITimer
//{
//	ITimerHandler* rec;
//	bool started;
//	uint16_t timeus;
//public:
//	Timer();
//	static Timer* Instance();
	void TimerStop();
	//void Start(ITimerHandler* t);
	void TimerStart();
	void TimerInit();
	bool TimerIsStarted();
	//void SetReceiver(ITimerHandler* pkeg);
	//friend void TIM2_IRQHandler();
	void TimerSetTimeUs(uint16_t timeus);
//};



#endif
