/*
 * slave-rtu.cpp
 *
 *  Created on: Jul 17, 2013
 *      Author: agu
 */
#include <string.h>
#include "slave-rtu.h"
#include "CFlash.h"
#include "bitops.h"
#include "crc.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_crc.h"

/**
 *
 *
 *
 */

uint16_t *mram = (uint16_t*)RAM_START;

static int16_t _coil_length;
static uint16_t _bit_input_length;
static uint16_t _short_input_length;
static uint16_t _holding_length;

static const uint16_t _BUFF_LENGTH = 256;
const char* _buff_rx;
uint8_t _address;
char _buff_tx[_BUFF_LENGTH];
uint8_t _bit_inputs[SLAVERTU_GETBYTES(SLAVERTU_BITINPUTS)];
uint16_t _short_inputs[SLAVERTU_SHORTINPUTS];
uint8_t _coils[SLAVERTU_GETBYTES(SLAVERTU_COILS)];
uint16_t _holdings[SLAVERTU_HOLDINGS];



SlaveRtu::SlaveRtu(RS485 & usart, uint8_t address) :
		_usart(usart),resolversCount(0) {
	_buff_rx = NULL;
	_address = address;

	//_bit_inputs = NULL;
	_bit_input_length = 0;

	//_short_inputs = NULL;
	_short_input_length = 0;

	//_coils = NULL;
	_coil_length = 0;

	//_holdings = NULL;
	_holding_length = 0;

	this->initBitInputs(SLAVERTU_BITINPUTS);
	this->initShortInputs(SLAVERTU_SHORTINPUTS);
	this->initCoils(SLAVERTU_COILS);
	this->initHoldings(SLAVERTU_HOLDINGS);

	uint16_t old[2];
	CFlash::Read16(old,1);
	setHolding(0,old[0]);
	setAddress(old[0]);
}

SlaveRtu::~SlaveRtu() {
//	free(_bit_inputs);
//	free(_short_inputs);
//
//	free(_coils);
//	free(_holdings);
}

//****************Init*****************************//

void SlaveRtu::initBitInputs(uint16_t length) {
	//if (_bit_inputs) free(_bit_inputs);

	_bit_input_length = length;
	uint8_t bit_input_byte_size = SLAVERTU_GETBYTES(length);
	//_bit_inputs = (uint8_t *) malloc(bit_input_byte_size * sizeof(uint8_t));

	assert_param(_bit_inputs);
	memset(_bit_inputs, 0, bit_input_byte_size);
}

void SlaveRtu::initShortInputs(uint16_t length) {
	//if (_short_inputs) free(_short_inputs);

	_short_input_length = length;
	//_short_inputs = (uint16_t *) malloc(_short_input_length * sizeof(uint16_t));
	//assert_param(_short_inputs);
	memset(_short_inputs, 0, _short_input_length * sizeof(uint16_t));
}

void SlaveRtu::initCoils(uint16_t length) {
	//if (_coils) free(_coils);

	_coil_length = length;
	uint8_t coils_byte_size = SLAVERTU_GETBYTES(length);
	//_coils = (uint8_t *) malloc(coils_byte_size * sizeof(uint8_t));
	assert_param(_coils);
	memset(_coils, 0, coils_byte_size);
}

void SlaveRtu::initHoldings(uint16_t length) {
	//if (_holdings) free(_holdings);

	_holding_length = length;
	//_holdings = (uint16_t *) malloc(_holding_length * sizeof(uint16_t));
	assert_param(_holdings);
	memset(_holdings, 0, _holding_length * sizeof(uint16_t));
}

void SlaveRtu::init(IWriteReg** writeResolvers,int resolversCount) {
	//_usart.init();
	this->writeResolvers = writeResolvers;
	this->resolversCount = resolversCount;
}

void SlaveRtu::setAddress(uint8_t address) {
	_address = address;
}

void SlaveRtu::handler(const char* pbuf, uint16_t length_rx) {
		_buff_rx = pbuf;
		//slaveRtu_t* rxStruct = (slaveRtu_t*)_buff_rx;
		//rxStruct->StartAddr = swap16(rxStruct->StartAddr);

		memset(_buff_tx, 0, _BUFF_LENGTH);

		do {
			if (length_rx < 4 || length_rx >= _BUFF_LENGTH) break;
			if (_buff_rx[0] && _buff_rx[0] != 0x01 && _buff_rx[0] != _address) break;
			if (this->checkFrameCrc(_buff_rx, length_rx) == false) break;

			_buff_tx[0] = _buff_rx[0];
			_buff_tx[1] = _buff_rx[1];	// function code

			uint8_t length_tx = 1;
			uint8_t exception = 0;

			switch (_buff_rx[1]) {
			case 0x01:
				exception = onReadCoils(&length_tx);
				break;
			case 0x02:
				exception = onReadBitInputs(&length_tx);
				break;
			case 0x03:
				exception = onReadHoldings(&length_tx);
				break;
			case 0x04:
				exception = onReadShortInputs(&length_tx);
				break;
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
				exception = 0x01;
				break;
			}

			if (exception) {
				_buff_tx[1] += 0x80;
				_buff_tx[2] = exception;
				length_tx = 3;
			}

			if (_buff_tx[0]) this->appendCrcAndReply(length_tx);
		} while (false);

		length_rx = 0;

}


bool SlaveRtu::checkFrameCrc(const char *p, uint8_t length) {
	// Enable CRC clock
	uint32_t crc0 = crc.calc(p, length - 2);
	uint32_t crc1 = p[length - 2] | (p[length - 1] << 8);
	return crc0 == crc1;
	return true;
}

void SlaveRtu::appendCrcAndReply(uint8_t length_tx) {
	uint16_t v = crc.calc(_buff_tx, length_tx);
	_buff_tx[length_tx] = lowByte(v);
	_buff_tx[length_tx + 1] = highByte(v);
	//_usart.write(_buff_tx, length_tx + 2);
	//_usart.flush();
	_usart.Send(_buff_tx,length_tx+2);
}

//****************get set*****************************//
void SlaveRtu::setCoil(uint16_t index, BitAction state) {
	assert_param(index < _coil_length);
	bitWrite(_coils[index >> 3], index & 0x07, state == Bit_SET);
}

BitAction SlaveRtu::getCoil(uint16_t index) {
	assert_param(index < _coil_length);
	return bitRead(_coils[index >> 3],index & 0x07) ? Bit_SET : Bit_RESET;
}

uint8_t SlaveRtu::onReadCoils(uint8_t * p_length_tx) {

	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d0) return 0x03;

	uint16_t address = make16(_buff_rx[2], _buff_rx[3]);
	if (address + length > _coil_length) return 0x02;

	for (uint16_t i = 0; i < length; i++) {
		bitWrite(_buff_tx[3 + (i >> 3)], i & 0x07,
			this->getCoil(address + i) == Bit_SET);
	}

	_buff_tx[2] = (length + 7) >> 3;

	*p_length_tx = 3 + _buff_tx[2];
	return 0;
}

void SlaveRtu::setBitInput(uint16_t index, BitAction state) {
	assert_param(index < _bit_input_length);
	bitWrite(_bit_inputs[index >> 3], index & 0x07, state == Bit_SET);
}

BitAction SlaveRtu::getBitInput(uint16_t index) {
	assert_param(index < _bit_input_length);
	return bitRead(_bit_inputs[index >> 3], index & 0x07) ? Bit_SET : Bit_RESET;
}

uint8_t SlaveRtu::onReadBitInputs(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d0) return 0x03;

	uint16_t address = make16(_buff_rx[2], _buff_rx[3]);
	if (address + length > _bit_input_length) return 0x02;

	if (updateBitInputs(address, length)) return 0x04;

	for (uint16_t i = 0; i < length; i++)
		bitWrite(_buff_tx[3 + (i >> 3)], i & 0x07,
			this->getBitInput(address + i) == Bit_SET);

	_buff_tx[2] = (length + 7) >> 3;

	*p_length_tx = 3 + _buff_tx[2];
	return 0;
}

uint8_t SlaveRtu::onWriteSingleCoil(uint8_t * p_length_tx) {
	uint16_t val = make16(_buff_rx[4], _buff_rx[5]);
	if (val && val != 0xff00) return 0x03;

	uint16_t address = make16(_buff_rx[2], _buff_rx[3]);
	if (address >= _coil_length) return 0x02;

	this->setCoil(address, val ? Bit_SET : Bit_RESET);

	if (updateCoils(address, 1)) return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

uint8_t SlaveRtu::onWriteMultipleCoils(uint8_t length_rx, uint8_t * p_length_tx)
{
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07b0) return 0x03;
	if (length_rx - 9 != _buff_rx[6]) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _coil_length) return 0x02;

	for (uint16_t i = 0; i < length; i++)
		this->setCoil(index + i,
		bitRead(_buff_rx[7 + (i >> 3)], i & 0x07) ? Bit_SET : Bit_RESET);

	if (updateCoils(index, length)) return 0x04;

	memcpy(_buff_tx + 2, _buff_rx + 2, 4);
	*p_length_tx = 6;
	return 0;
}

uint8_t SlaveRtu::onReadShortInputs(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);
	if (index + length > _short_input_length) return 0x02;

	if (updateShortInputs(index, length)) return 0x04;

	for (uint8_t i = 0; i < length; i++) {
		_buff_tx[3 + i + i] = highByte(_short_inputs[index + i]);
		_buff_tx[4 + i + i] = lowByte(_short_inputs[index + i]);
	}

	_buff_tx[2] = length * 2;
	*p_length_tx = _buff_tx[2] + 3;
	return 0;
}

void SlaveRtu::setShortInput(uint16_t index, uint16_t val) {
	assert_param(index < _short_input_length);
	_short_inputs[index] = val;
}

uint16_t SlaveRtu::getShortInput(uint16_t index) {
	assert_param(index < _short_input_length);
	return _short_inputs[index];
}

uint8_t SlaveRtu::onReadHoldings(uint8_t * p_length_tx) {
	uint16_t length = make16(_buff_rx[4], _buff_rx[5]);
	if (!length || length > 0x07d) return 0x03;

	uint16_t index = make16(_buff_rx[2], _buff_rx[3]);

	if (index>=0 && index < _holding_length)
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
	if (index==DEVADDR_OFFSET)
	{
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
	}
	else if (index==RESET_REG_OFFSET)
	{
		if (val > 0)
			RCC_ClearFlag();
		result = true;
	}
	else
	{
		for(int i=0;i<resolversCount;i++)
		{
			if (writeResolvers[i]->IsValid(index,val))
			{
				writeResolvers[i]->Write(index,val);
				result = true;
				break;
			}
		}
	}
	return result;
}

bool SlaveRtu::setHolding(uint16_t index, uint16_t val) {
	assert_param(index < _holding_length);

	_holdings[index] = val;

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

