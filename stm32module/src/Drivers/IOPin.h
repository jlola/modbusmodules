#ifndef IOPIN_H
#define IOPIN_H

//#include "IIOPin.h"

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_syscfg.h"

typedef enum
{
	IOInput,
	IOOutput
} IODirection;

class IOPin// : public IIOPin
{
	IODirection dir;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
public:
	IOPin(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,IODirection pdir);
	void Init();
	void Set(bool set);
	bool IsSet();
};

#endif
