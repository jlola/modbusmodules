/*
 * RFIDRegs.h
 *
 *  Created on: 13. 11. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_RFID_RFIDREGS_H_
#define DRIVERS_RFID_RFIDREGS_H_

#include "IWriteReg.h"
#include "IUSART.h"
#include "IUSARTHandler.h"
#include "IModbusSlave.h"
#include "ITimer.h"

#define MODBUS_DATA_SIZE 50

typedef struct
{
	uint16_t NewDataFlag;
	uint8_t buffer[MODBUS_DATA_SIZE];
} SRFIDRegs;

class RFIDRegs_test;

class RFIDRegs : public IWriteReg, public IUSARTHandler, public ITimerHandler
{
	friend class RFIDRegs_test;
public:
	bool newToHoldings;
	SRFIDRegs regs;
	uint8_t bufferIndex;
	IUSART *usart;
	uint16_t offset;
	IModbusSlave* slave;
	ITimer* timer;
public:
	RFIDRegs(IUSART *usart, IModbusSlave* slave, ITimer* timer);
	~RFIDRegs();

	void OnHWTimer(uint8_t us);
	void OnReceiveData(char data);
	void OnReceiveData(uint8_t* data, uint16_t size, bool completed);
	void SendingCompleted();
	void ReceiverTimeout();
	bool IsValid(uint16_t index, uint16_t value);
	bool Write(uint16_t index, uint16_t value);
	bool Refresh();
	EDeviceType GetDeviceType();
	EModbusFunctions GetModbusFunc();
	uint16_t GetSizeInWords();
	void SetOffset(uint16_t offset);
};

#endif /* DRIVERS_RFID_RFIDREGS_H_ */
