/*
 * IUSART.h
 *
 *  Created on: 26. 11. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_USART_IUSART_H_
#define DRIVERS_USART_IUSART_H_

#include "IUSARTHandler.h"

class IUSART
{
public:
	virtual void SetTimeOut(uint8_t bits)=0;
	virtual void EnableTimeout(bool enable)=0;
	virtual void Enable(bool enable);
	virtual void SetHandler(IUSARTHandler* handler)=0;
	virtual void ReceiveEnable(bool enable)=0;
	virtual void Send(const char* pchar,size_t len)=0;
	virtual void Send(char pchar)=0;
	virtual void HWControlledDE(bool enable)=0;
	virtual bool IsIdle()=0;
	virtual bool IsBusy()=0;
	virtual ~IUSART(){}
};


#endif /* DRIVERS_USART_IUSART_H_ */
