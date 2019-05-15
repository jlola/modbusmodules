/*
 * USARTHandler.h
 *
 *  Created on: 25. 11. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_USART_IUSARTHANDLER_H_
#define DRIVERS_USART_IUSARTHANDLER_H_


class IUSARTHandler
{
public:
	virtual void OnReceiveData(char data)=0;
	virtual void OnReceiveData(uint8_t* data, uint16_t size, bool completed)=0;
	virtual void SendingCompleted()=0;
	virtual void ReceiverTimeout()=0;
	virtual ~IUSARTHandler(){};
};


#endif /* DRIVERS_USART_IUSARTHANDLER_H_ */
