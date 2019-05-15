/*
 * slave-rtu.h
 *
 *  Created on: Jul 17, 2013
 *      Author: agu
 */

#ifndef SLAVE_RTU_H_
#define SLAVE_RTU_H_

#define RAM_INDEX 0x2000
#define RAM_START 0x20000000L
#define RAM_SIZE  0x1000

#include <stdint-gcc.h>

#ifndef TEST
#include "stm32f0xx_gpio.h"
#include "stm32f0xx.h"
#endif

#include "rs485.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define BROADCAST_ADDRESS 	0x01

#define TX_ADDRESS_POS 		0x00
#define TX_FUNC_POS 		0x01

#define MODBUS_FUNC_READ_HOLDINGS		0x03
//#include "IModbusRegister.h"

//using namespace srutil;
//
//#ifdef SRUTIL_DELEGATE_PREFERRED_SYNTAX
//typedef srutil::delegate<uint8_t (uint16_t index, uint16_t length, uint16_t* buffer)> OnUpdateDelegate;
//#else
// typedef srutil::delegate<void, uint16_t index, uint16_t length> OnReceived;
//#endif

/**
 * SlaveRtu class
 * get me
 */
#include "Settings.h"
#include "IModbusObject.h"
#include "IModbusSlave.h"

#ifndef SLAVERTU_BITINPUTS
#define SLAVERTU_BITINPUTS 100
#endif
#ifndef SLAVERTU_SHORTINPUTS
#define SLAVERTU_SHORTINPUTS 100
#endif
#ifndef SLAVERTU_COILS
#define SLAVERTU_COILS 100
#endif
#ifndef SLAVERTU_HOLDINGS
#define SLAVERTU_HOLDINGS 128
#endif

#define SLAVERTU_GETBYTES(x) (x + 7) >> 3

class SlaveRtu : public IModbusSlave
{
	uint8_t _address;
	int resolversCount;
	IModbusObject** writeResolvers;
	RS485 & _usart;
public:
	SlaveRtu(RS485 & usart, uint8_t address);
	virtual ~SlaveRtu();

	void ReceiveData();

	uint16_t GetMaxIndex();

	void init(IModbusObject* writeResolvers[],size_t resolversCount);
	void setAddress(uint8_t address);
	void handler(const uint8_t* pchar, uint16_t length_rx);

	void initHoldings(uint16_t length);

	bool setHoldingResolve(uint16_t index, uint16_t val);

	bool setHolding(uint16_t index, uint16_t val,bool alarm);
	bool setHoldings(uint16_t index, uint16_t* buffer,uint16_t length);
	uint16_t getHolding(uint16_t index);
protected:

	bool inline IsValidAddress(uint8_t address, uint16_t len);
	virtual uint8_t updateHoldings(uint16_t index, uint16_t length)
	{
		//if (OnUpdateHoldings!=0)
		//	return OnUpdateHoldings(index,length,this->_holdings);
		return 0;
	};

private:

	bool checkFrameCrc(const char *p, uint8_t length);
	void appendCrcAndReply(uint8_t length_tx, bool withDelay);

	uint8_t onReadHoldings(uint8_t * p_length_tx);
	uint8_t onWriteSingleHolding(uint8_t * p_length_tx);
	uint8_t onWriteMultipleHoldings(uint8_t length_rx, uint8_t * p_length_tx);
};

#endif /* SLAVE_RTU_H_ */
