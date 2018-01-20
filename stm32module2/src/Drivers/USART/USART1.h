#ifndef CUSART2_H_
#define CUSART2_H_

#include <stdint.h>
#include "USARTBase.h"

class CUSART1 : public USARTBase
{

private:
	CUSART1();
public:
	void SetTimeOut(uint8_t bits);
	void EnableTimeout(bool enable);
	void Init(uint32_t speed);
	void Enable(bool enable);
	static CUSART1* Instance();
};



#endif
