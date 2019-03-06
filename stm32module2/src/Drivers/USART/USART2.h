/*
 * USART2.h
 *
 *  Created on: 19. 11. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_USART_USART2_H_
#define DRIVERS_USART_USART2_H_

#include "USARTBase.h"

class CUSART2 : public USARTBase
{
public:
	CUSART2();

	void SetTimeOut(uint8_t bits);
	void EnableTimeout(bool enable);
	void Init(uint32_t speed);
	void Enable(bool enable);
	void ReceiveEnable(bool enable);
	static USARTBase* Instance();

	virtual ~CUSART2();
};

#endif /* DRIVERS_USART_USART2_H_ */
