#ifndef ITIMERHANDLER_H
#define ITIMERHANDLER_H

#include <stdint.h>

class ITimerHandler
{
public:
	virtual void OnHWTimer(uint8_t us)=0;
	virtual ~ITimerHandler(){}
};


#endif
