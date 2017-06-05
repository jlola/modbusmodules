#ifndef IIOPIN_H
#define IIOPIN_H

class IIOPin
{
public:
	virtual void Init()=0;
	virtual void Set(bool set)=0;
	virtual bool IsSet()=0;
};

#endif
