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
#include "stm32f0xx_gpio.h"
#include "stm32f0xx.h"
#include "rs485.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

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
#include "IWriteReg.h"

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

class SlaveRtu// : public IModbusRegister
{
//	 typedef struct {
//		 uint8_t Address;
//		 uint8_t Function;
//		 uint16_t StartAddr;
//		 uint16_t CountRegs;
//
//	 } slaveRtu_t;
	int resolversCount;
	IWriteReg** writeResolvers;
public:
	SlaveRtu(RS485 & usart, uint8_t address);
	virtual ~SlaveRtu();

	void init(IWriteReg* writeResolvers[],int resolversCount);
	void setAddress(uint8_t address);
	void handler(const char* pchar, uint16_t length_rx);
	void handleTimIrq();

	void initBitInputs(uint16_t length);
	void initShortInputs(uint16_t length);

	void initCoils(uint16_t length);
	void initHoldings(uint16_t length);

	void setBitInput(uint16_t index, BitAction state);
	BitAction getBitInput(uint16_t index);

	void setShortInput(uint16_t index, uint16_t val);
	uint16_t getShortInput(uint16_t index);

	void setCoil(uint16_t index, BitAction state);
	BitAction getCoil(uint16_t index);

	bool setHoldingResolve(uint16_t index, uint16_t val);

	bool setHolding(uint16_t index, uint16_t val);
	uint16_t getHolding(uint16_t index);

//	OnUpdateDelegate OnUpdateHoldings;
//	OnUpdateDelegate OnUpdateShortInputs;
//	OnUpdateDelegate OnUpdateCoils;
//	OnUpdateDelegate OnUpdateBitInputs;

protected:


	virtual uint8_t updateBitInputs(uint16_t index, uint16_t length)
	{
		return 0;
	};
	virtual uint8_t updateShortInputs(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateCoils(uint16_t index, uint16_t length) {return 0;};
	virtual uint8_t updateHoldings(uint16_t index, uint16_t length)
	{
		//if (OnUpdateHoldings!=0)
		//	return OnUpdateHoldings(index,length,this->_holdings);
		return 0;
	};

private:
	RS485 & _usart;


	bool checkFrameCrc(const char *p, uint8_t length);
	void appendCrcAndReply(uint8_t length_tx);

	uint8_t onReadBitInputs(uint8_t * p_length_tx);
	uint8_t onReadShortInputs(uint8_t * p_length_tx);

	uint8_t onReadCoils(uint8_t * p_length_tx);
	uint8_t onWriteSingleCoil(uint8_t * p_length_tx);
	uint8_t onWriteMultipleCoils(uint8_t length_rx, uint8_t * p_length_tx);

	uint8_t onReadHoldings(uint8_t * p_length_tx);
	uint8_t onWriteSingleHolding(uint8_t * p_length_tx);
	uint8_t onWriteMultipleHoldings(uint8_t length_rx, uint8_t * p_length_tx);
};

#endif /* SLAVE_RTU_H_ */
