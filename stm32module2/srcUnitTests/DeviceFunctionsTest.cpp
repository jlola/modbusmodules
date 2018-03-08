/*
 * DeviceFunctionsTest.cpp
 *
 *  Created on: 2. 12. 2017
 *      Author: Libor
 */

#include <DeviceFunctionsTest.h>
#include "DeviceFunctions.h"
#include "IModbusSlave.h"
#include "IModbusObjectFactory.h"
#include "ModbusObjectsMockGenerator.h"
#include "SlaveMockHelper.h"

namespace AF {

using namespace fakeit;

DeviceFunctionsTest::DeviceFunctionsTest() {
	// TODO Auto-generated constructor stub

}

TEST_F(DeviceFunctionsTest,CheckInstancesAndProcess)
{
	SlaveMockHelper slaveMockHelper;
	Mock<IModbusSlave> & slaveMockInst = slaveMockHelper.GetSlaveMock();
	IModbusSlave* slave = &slaveMockInst.get();
	Mock<IModbusObjectFactory> factoryMock;

	IModbusObjectFactory* factory = &factoryMock.get();

	ModbusObjectsMockGenerator generator;
	Mock<IModbusObject> inputRegs1;
	Mock<IModbusObject> inputRegs2;
	Mock<IModbusObject> inputRegs3;
	Mock<IModbusObject> outputRegs;
	Mock<IModbusObject> onewireRegs;
	Mock<IModbusObject> rfidRegs;
	generator.CreateInput1(inputRegs1);
	generator.CreateInput2(inputRegs2);
	generator.CreateInput3(inputRegs3);
	generator.CreateOutput1(outputRegs);
	generator.CreateDS18B20(onewireRegs);
	generator.CreateRFIDRegs(rfidRegs);
	IModbusObject** modbusobjects;
	int modbusobjectsCount = 6;
	modbusobjects = new IModbusObject*[modbusobjectsCount];
	modbusobjects[0] = dynamic_cast<IModbusObject*>(&inputRegs1.get());
	modbusobjects[1] = dynamic_cast<IModbusObject*>(&inputRegs2.get());
	modbusobjects[2] = dynamic_cast<IModbusObject*>(&inputRegs3.get());
	modbusobjects[3] = dynamic_cast<IModbusObject*>(&outputRegs.get());
	modbusobjects[4] = dynamic_cast<IModbusObject*>(&onewireRegs.get());
	modbusobjects[5] = dynamic_cast<IModbusObject*>(&rfidRegs.get());

	When(Method(factoryMock,ModbusObjects)).AlwaysReturn(modbusobjects);
	When(Method(factoryMock,ModbusObjectsCount)).AlwaysReturn(modbusobjectsCount);

	When(Method(slaveMockInst,setHolding).Using(TYPE_DEFS_OFFSET,TYPE_DEFS_START_ADDRESS)).AlwaysReturn();
	When(Method(slaveMockInst,setHolding).Using(COUNT_OF_TYPES_OFFSET,4)).AlwaysReturn();
	When(Method(slaveMockInst,init)).AlwaysReturn();

	DeviceFunctions df( slave, factory);
	df.Process();

	Verify(Method(slaveMockInst,init)).Exactly(1_Times);
	Verify(Method(inputRegs1,Refresh)).Exactly(1_Times);
	Verify(Method(inputRegs2,Refresh)).Exactly(1_Times);
	Verify(Method(inputRegs3,Refresh)).Exactly(1_Times);
	Verify(Method(outputRegs,Refresh)).Exactly(1_Times);
	Verify(Method(onewireRegs,Refresh)).Exactly(1_Times);
	Verify(Method(rfidRegs,Refresh)).Exactly(1_Times);

}

DeviceFunctionsTest::~DeviceFunctionsTest() {
	// TODO Auto-generated destructor stub
}


}
