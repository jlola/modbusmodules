/*
 * ModbusAddressManagerTests.h
 *
 *  Created on: 31. 10. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_MODBUS_MODBUSADDRESSMANAGERTESTS_H_
#define DRIVERS_MODBUS_MODBUSADDRESSMANAGERTESTS_H_

#include "ModbusAddressManager.h"
#include "gtest/gtest.h"
#include "fakeit.hpp"

namespace AF {

using namespace fakeit;

class ModbusAddressManagerTests : public testing::Test{
public:
	ModbusAddressManagerTests();
	virtual ~ModbusAddressManagerTests();
};

}

#endif /* DRIVERS_MODBUS_MODBUSADDRESSMANAGERTESTS_H_ */
