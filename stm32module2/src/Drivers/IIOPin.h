#ifndef IIOPIN_H
#define IIOPIN_H

#include <stdint.h>

class IIOPin
{
public:
	virtual void Init()=0;
	virtual void Set(bool set)=0;
	virtual bool IsSet()=0;
	virtual uint8_t GetPinNumber()=0;
};

#endif
