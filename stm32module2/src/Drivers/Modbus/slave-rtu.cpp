#ifndef UNITTEST

/*
 * slave-rtu.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: agu
 */
#include <string.h>
#include <stdlib.h>
#include "slave-rtu.h"
#include "CFlash.h"
#include "bitops.h"
#include "crc.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_crc.h"
#include "diag/Trace.h"

/**
 *
 *
 *
 */

uint16_t *mram = (uint16_t*)RAM_START;

static uint16_t _holding_length;

static const uint16_t _BUFF_LENGTH = 256;
const uint8_t* _buff_rx;
char _buff_tx[_BUFF_LENGTH];
uint16_t _holdings[SLAVERTU_HOLDINGS];



SlaveRtu::SlaveRtu(RS485 & usart, uint8_t address) :
		resolversCount(0),writeResolvers(NULL),_usart(usart) {
	_buff_rx = NULL;
	_address = address;

	//_holdings = NULL;
	_holding_length = 0;

	this->initHoldings(SLAVERTU_HOLDINGS);

	srand(_address);
	setHolding(0,_address,true);
}

void SlaveRtu::ReceiveData()
{
	if (_usart.PacketCompleted())
	{
		uint8_t* buffer=NULL;
		uint16_t size = 0;
		_usart.ReadBuffer(buffer,size);
		//Watchdog::ResetCounter();
		handler(buffer,size);
		_usart.RecEnable(true);
	}
}

SlaveRtu::~SlaveRtu() {
//	free(_bit_inputs);
//	free(_short_inputs);
//
//	free(_coils);
//	free(_holdings);
}

//****************Init*****************************//

uint16_t SlaveRtu::GetMaxIndex()
{
	return (uint16_t)SLAVERTU_HOLDINGS;
}

void SlaveRtu::initHoldings(uint16_t length) {
	//if (_holdings) free(_holdings);
	_holding_length = length;
	//_holdings = (uint16_t *) malloc(_holding_length * sizeof(uint16_t));
	assert_param(_holdings);
	memset(_holdings, 0, _holding_length * sizeof(uint16_t));
}

void SlaveRtu::init(IModbusObject* writeResolvers[], size_t resolversCount) {
	//_usart.init();
	this->writeResolvers = writeResolvers;
	this->resolversCount = resolversCount;
}

void SlaveRtu::setAddress(uint8_t address) {
	_address = address;
}

bool SlaveRtu::IsValidAddress(uint8_t recAddress, uint16_t len)
{
	if (recAddress == _address)
		return true;
	if (recAddress == BROADCAST_ADDRESS && _holdings[CHANGE_FLAG]!=0 && len == 8)
		return true;
	return false;
}

void SlaveRtu::handler(const uint8_t* pbuf, uint16_t length_rx) {
		_buff_rx = pbuf;
		uint8_t length_tx = 0;
		memset(_buff_tx, 0, _BUFF_LENGTH);

		do {
			if (length_rx < 4 || length_rx >= _BUFF_LENGTH) break;
			if (!IsValidAddress(_buff_rx[0],length_rx)) break;
			if (!checkFrameCrc((const char*)_buff_rx, length_rx)) break;

			_buff_tx[TX_ADDRESS_POS] = _buff_rx[TX_ADDRESS_POS];
			_buff_tx[TX_FUNC_POS] = _buff_rx[TX_FUNC_POS];	// function code

			length_tx = 1;
			uint8_t exception = 0;

			switch (_buff_rx[TX_FUNC_POS]) {
//			case 0x01:
//				exception = onReadCoils(&length_tx);
//				break;
//			case 0x02:
//				exception = onReadBitInputs(&length_tx);
//				break;
			case MODBUS_FUNC_READ_HOLDINGS:
				exception = onReadHoldings(&length_tx);
				break;
//			case 0x04:
//				exception = onReadShortInputs(&length_tx);
//				break;
			//case 0x05:
			//	exception = onWriteSingleCoil(&length_tx);
			//	break;
			case 0x06:
				exception = onWriteSingleHolding(&length_tx);
				break;
//			case 0x0f:
//				exception = onWriteMultipleCoils(length_rx, &length_tx);
//				break;
			case 0x10:
				exception = onWriteMultipleHoldings(length_rx, &length_tx);
				break;
			default:
				//if (_buff_rx[TX_ADDRESS_POS]!=BROADCAST_ADDRESS)
				//	exception = 0x01;
				//else
				return;
			}

			if (exception) {
				_buff_tx[1] += 0x80;
				_buff_tx[2] = exception;
				length_tx = 3;
			}
		} while (false);

	if (_buff_tx[TX_ADDRESS_POS] > 0)
	{
		bool delayAllowed =	_buff_rx[TX_ADDRESS_POS]==BROADCAST_ADDRESS;
		this->appendCrcAndReply(length_tx,delayAllowed);
	}

	length_rx = 0;
}


bool SlaveRtu::checkFrameCrc(const char *p, uint8_t length) {
	// Enable CRC clock
	uint32_t crc0 = crc.calc(p, length - 2);
	uint32_t crc1 = p[length - 2] | (p[length - 1] << 8);
	return crc0 == crc1;
	return true;
}

void SlaveRtu::appendCrcAndReply(uint8_t length_tx, bool withDelay) {
	uint16_t v = crc.calc(_buff_tx, length_tx);
	_buff_tx[length_tx] = lowByte(v);
	_buff_tx[length_tx + 1] = highByte(v);

	_usart.Send(_buff_tx,length_tx+2,withDelay);
}

//****************get set*****************************//

uint8_t SlaveRtu::onReadHoldings(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);

	if (index < _holding_length)
	{
		if (index + length > _holding_length) return 0x02;

		for (uint8_t i = 0; i < length; i++) {
			_buff_tx[3 + i + i] = highByte(_holdings[index + i]);
			_buff_tx[4 + i + i] = lowByte(_holdings[index + i]);
		}
	}
	else if ( index >= RAM_INDEX)
	{
		index = index - RAM_INDEX;

		if (index + length > (RAM_INDEX + RAM_SIZE)) return 0x02;

		for (uint8_t i = 0; i < length; i++) {
			_buff_tx[3 + i + i] = highByte(mram[i+index]);
			_buff_tx[4 + i + i] = lowByte(mram[i+index]);
		}
	}
	else return 0x02;

	_buff_tx[2] = length * 2;
	*p_length_tx = _buff_tx[2] + 3;
	return 0;
}

bool SlaveRtu::setHoldingResolve(uint16_t index, uint16_t val)
{
	bool result = false;
	switch(index)
	{
		case DEVADDR_OFFSET:
			_holdings[index] = val;
			setAddress(val);
			uint16_t old[2];
			CFlash::Read16(old,1);
			if (old[0]!=val)
			{
				old[0] = val;
				CFlash::Write16(old,1);
			}
			result = true;
			break;
		case RESET_REG_OFFSET:
			if (val > 0)
				RCC_ClearFlag();
			result = true;
			break;
		case CHANGE_FLAG:
			if (val > 0)
				_holdings[CHANGE_FLAG] = 0;
			result = true;
			break;
		default:
			for(int i=0;i<resolversCount;i++)
			{
				if (writeResolvers[i]->IsValid(index,val))
				{
					writeResolvers[i]->Write(index,val);
					result = true;
					break;
				}
			}
			break;
	}
	return result;
}

bool SlaveRtu::setHolding(uint16_t index, uint16_t val,bool alarm) {
	assert_param(index < _holding_length);

	if (_holdings[index] != val)
	{
		_holdings[index] = val;
		if (alarm)
			_holdings[CHANGE_FLAG] = 1;
	}

	return true;
}

bool SlaveRtu::setHoldings(uint16_t index, uint16_t* buffer,uint16_t length)
{
	memcpy(&_holdings[index],buffer,length*2);
	return true;
}

uint16_t SlaveRtu::getHolding(uint16_t index) {
	assert_param(index < _holding_length);
	return _holdings[index];
}

uint8_t SlaveRtu::onWriteSingleHolding(uint8_t * p_length_tx) {
	uint16_t val = make16(_buff_rx[4], _buff_rx[5]);

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index >= _holding_length) return 0x02;

	if (!this->setHoldingResolve(index, val)) return 0x04;
	//this->setAddress(_buff_rx[5]);

	if (updateHoldings(index, 1)) return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

uint8_t SlaveRtu::onWriteMultipleHoldings(uint8_t length_rx,
	uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x7b) return 0x03;
	if (length_rx - 9 != _buff_rx[6]) return 0x03;
	if (length != _buff_rx[6] >> 1) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _holding_length) return 0x02;

	for (uint8_t i = 0; i < length; i++) {
		_holdings[index + i] =
		make16(_buff_rx[7 + i + i], _buff_rx[8 + i + i]);
	}

	if (updateHoldings(index, length)) return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

#endif
