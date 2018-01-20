/*
 * ModbusAddressManagerTests.cpp
 *
 *  Created on: 31. 10. 2017
 *      Author: Libor
 */

#ifdef UNITTEST

#include "Settings.h"
#include "ModbusAddressManagerTests.h"
#include "ModbusObjectsMockGenerator.h"
#include <SlaveMockHelper.h>


namespace AF {

ModbusAddressManagerTests::ModbusAddressManagerTests() {
}

TEST_F(ModbusAddressManagerTests,ComputeAddressesTest)
{
	SlaveMockHelper slaveMock;
	IModbusSlave & mockModbusSlaveInst = slaveMock.GetSlaveMock().get();
	Mock<IModbusObject> input1mock;
	Mock<IModbusObject> input2mock;
	Mock<IModbusObject> input3mock;
	Mock<IModbusObject> outputmock;
	Mock<IModbusObject> ds18b20mock;
	Mock<IModbusObject> rfidmock;
	ModbusObjectsMockGenerator generator;


	generator.CreateInput1(input1mock);
	generator.CreateInput2(input2mock);
	generator.CreateInput3(input3mock);
	generator.CreateOutput1(outputmock);
	generator.CreateDS18B20(ds18b20mock);
	generator.CreateRFIDRegs(rfidmock);

	IModbusObject* modbusobjects[6];
	modbusobjects[0] = &input1mock.get();
	modbusobjects[1] = &input2mock.get();
	modbusobjects[2] = &input3mock.get();
	modbusobjects[3] = &outputmock.get();
	modbusobjects[4] = &ds18b20mock.get();
	modbusobjects[5] = &rfidmock.get();

	ModbusAddressManager manager(&mockModbusSlaveInst,modbusobjects,sizeof(modbusobjects) / sizeof(IModbusObject*));
	manager.ComputeAddresses();
}

ModbusAddressManagerTests::~ModbusAddressManagerTests() {
	// TODO Auto-generated destructor stub
}

}

#endif
