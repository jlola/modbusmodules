#ifndef ITIMER_H
#define ITIMER_H

#include "ITimerHandler.h"
#include <stdint.h>

class ITimer
{
public:
	virtual void Stop()=0;
	virtual void Start(ITimerHandler* t)=0;
	virtual void Start()=0;
	virtual void Init()=0;
	virtual void SetReceiver(ITimerHandler* pkeg)=0;
	virtual bool IsStarted()=0;
	virtual void SetTimeUs(uint16_t timeus)=0;
};

#endif
