#ifndef IOPIN_H
#define IOPIN_H

//#include "IIOPin.h"

#ifndef TEST

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_syscfg.h"

#endif

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
	uint8_t pinNumber;
public:
	IOPin(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,IODirection pdir,uint8_t pinNumber);
	void Init();
	void Set(bool set);
	bool IsSet();
	uint8_t GetPinNumber();
};

#endif
