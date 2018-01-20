/*
 * ModbusObjectFactory.h
 *
 *  Created on: 1. 12. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_MODBUS_MODBUSOBJECTFACTORY_H_
#define DRIVERS_MODBUS_MODBUSOBJECTFACTORY_H_

#include "IOPin.h"
#include "BinInputRegs.h"
#include "OutputReg.h"
#include "IModbusObjectFactory.h"
#include "IOneWireThread.h"
#include "OneWireManager.h"
#include "IUSART.h"
#include "ITimer.h"
#include "RFID/RFIDRegs.h"

class ModbusObjectFactory : public IModbusObjectFactory
{
	IModbusSlave * slave;
	OneWireManager* owmanager;
	size_t modbusobjectsCount;
	IModbusObject** modbusobjects;
public:
	ModbusObjectFactory(IModbusSlave * pslave,
			IUSART* rfidusart,
			ITimer* rfidtimer,
			OneWireManager* owmanager);
	ModbusObjectFactory(IModbusSlave * pslave,
			OneWireManager* powmanager);

	IModbusObject** ModbusObjects();
	size_t ModbusObjectsCount();

	virtual ~ModbusObjectFactory();
};

#endif /* DRIVERS_MODBUS_MODBUSOBJECTFACTORY_H_ */
