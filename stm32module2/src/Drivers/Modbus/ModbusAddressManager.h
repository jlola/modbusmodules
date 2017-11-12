/*
 * ModbusAddressManager.h
 *
 *  Created on: 31. 10. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_MODBUS_MODBUSADDRESSMANAGER_H_
#define DRIVERS_MODBUS_MODBUSADDRESSMANAGER_H_

#include <stdint.h>
#include "IModbusObject.h"
#include "IModbusSlave.h"

typedef struct {
	uint16_t Type;
	uint16_t Count;
	uint16_t Address;
	uint16_t ReadFunc;
} STypeInfo;

class ModbusAddressManager {
	IModbusSlave* slave;
	IModbusObject** modbusobjects;
	int objectsCount;

	uint8_t GetObjectTypeCounts();
	void WriteTypeInfo(uint8_t addr, STypeInfo type);
public:
	ModbusAddressManager(IModbusSlave * slave,IModbusObject* modbusobjects[], size_t count);
	void ComputeAddresses();
	virtual ~ModbusAddressManager();
};

#endif /* DRIVERS_MODBUS_MODBUSADDRESSMANAGER_H_ */
