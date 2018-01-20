/*
 * IModbusObjectFactory.h
 *
 *  Created on: 1. 12. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_MODBUS_IMODBUSOBJECTFACTORY_H_
#define DRIVERS_MODBUS_IMODBUSOBJECTFACTORY_H_

#include "IModbusObject.h"
#include "stdint.h"
#include "stddef.h"

class IModbusObjectFactory
{
public:
	virtual IModbusObject** ModbusObjects() = 0;
	virtual size_t ModbusObjectsCount() = 0;
};



#endif /* DRIVERS_MODBUS_IMODBUSOBJECTFACTORY_H_ */
