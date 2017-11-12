/*
 * ModbusAddressManagerTests.cpp
 *
 *  Created on: 31. 10. 2017
 *      Author: Libor
 */

#ifdef UNITTEST

#include "Settings.h"
#include "ModbusAddressManagerTests.h"


namespace AF {

ModbusAddressManagerTests::ModbusAddressManagerTests() {
}

void ModbusAddressManagerTests::CreateInput(Mock<IModbusObject> & inputmock,uint16_t offset)
{
	When(Method(inputmock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeBinInputs);
	When(Method(inputmock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(inputmock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(inputmock,GetSizeInWords)).AlwaysReturn(5);
}

void ModbusAddressManagerTests::CreateOutput(Mock<IModbusObject> & outputmock,uint16_t offset)
{
	When(Method(outputmock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeBinOutputs);
	When(Method(outputmock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(outputmock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(outputmock,GetSizeInWords)).AlwaysReturn(3);
}

void ModbusAddressManagerTests::CreateDS18B20(Mock<IModbusObject> & mock,uint16_t offset)
{
	When(Method(mock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeDS18B20);
	When(Method(mock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(mock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(mock,GetSizeInWords)).AlwaysReturn(60);
}

TEST_F(ModbusAddressManagerTests,ComputeAddressesTest)
{
	Mock<IModbusSlave> mockModbusSlave;
	When(Method(mockModbusSlave,setHolding).Using(1,3)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(10,1)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(11,3)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(12,23)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(13,3)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(14,2)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(15,1)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(16,38)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(17,3)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(18,5)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(19,1)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(20,41)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(21,3)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(4,101)).AlwaysReturn();
	IModbusSlave & mockModbusSlaveInst = mockModbusSlave.get();

	Mock<IModbusObject> input1mock;
	Mock<IModbusObject> input2mock;
	Mock<IModbusObject> input3mock;
	Mock<IModbusObject> outputmock;
	Mock<IModbusObject> ds18b20mock;
	CreateInput(input1mock,23);
	CreateInput(input2mock,28);
	CreateInput(input3mock,33);
	CreateOutput(outputmock,38);
	CreateDS18B20(ds18b20mock,41);

	IModbusObject* modbusobjects[5];
	modbusobjects[0] = &input1mock.get();
	modbusobjects[1] = &input2mock.get();
	modbusobjects[2] = &input3mock.get();
	modbusobjects[3] = &outputmock.get();
	modbusobjects[4] = &ds18b20mock.get();

	ModbusAddressManager manager(&mockModbusSlaveInst,modbusobjects,sizeof(modbusobjects) / sizeof(IModbusObject*));
	manager.ComputeAddresses();
}

ModbusAddressManagerTests::~ModbusAddressManagerTests() {
	// TODO Auto-generated destructor stub
}

}

#endif
