/*
 * ModbusObjectsMockGenerator.cpp
 *
 *  Created on: 2. 12. 2017
 *      Author: Libor
 */

#include <ModbusObjectsMockGenerator.h>
#include "fakeit.hpp"

namespace AF {


ModbusObjectsMockGenerator::ModbusObjectsMockGenerator() {
	// TODO Auto-generated constructor stub
}

void ModbusObjectsMockGenerator::CreateInput1(Mock<IModbusObject> & inputmock)
{
	CreateInput(inputmock,OFFSET_INPUT1);
}

void ModbusObjectsMockGenerator::CreateInput2(Mock<IModbusObject> & inputmock)
{
	CreateInput(inputmock,OFFSET_INPUT2);
}

void ModbusObjectsMockGenerator::CreateInput3(Mock<IModbusObject> & inputmock)
{
	CreateInput(inputmock,OFFSET_INPUT3);
}

void ModbusObjectsMockGenerator::CreateOutput1(Mock<IModbusObject> & outputmock)
{
	CreateOutput(outputmock,OFFSET_OUTPUT1);
}

void ModbusObjectsMockGenerator::CreateDS18B20(Mock<IModbusObject> & ds18b20mock)
{
	CreateDS18B20(ds18b20mock,OFFSET_DS18B20);
}

void ModbusObjectsMockGenerator::CreateRFIDRegs(Mock<IModbusObject> & mock)
{
	CreateRFIDRegs(mock,OFFSET_RFIDREG);
}


void ModbusObjectsMockGenerator::CreateInput(Mock<IModbusObject> & mock,uint16_t offset)
{
	When(Method(mock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeBinInputs);
	When(Method(mock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(mock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(mock,GetSizeInWords)).AlwaysReturn(5);
	When(Method(mock,Refresh)).AlwaysReturn();
}

void ModbusObjectsMockGenerator::CreateOutput(Mock<IModbusObject> & mock,uint16_t offset)
{
	When(Method(mock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeBinOutputs);
	When(Method(mock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(mock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(mock,GetSizeInWords)).AlwaysReturn(3);
	When(Method(mock,Refresh)).AlwaysReturn();
}

Mock<IModbusObject> & ModbusObjectsMockGenerator::CreateDS18B20(Mock<IModbusObject> & mock,uint16_t offset)
{
	When(Method(mock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeDS18B20);
	When(Method(mock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(mock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(mock,GetSizeInWords)).AlwaysReturn(60);
	When(Method(mock,Refresh)).AlwaysReturn();
	return mock;
}

void ModbusObjectsMockGenerator::CreateRFIDRegs(Mock<IModbusObject> & mock,uint16_t offset)
{
	When(Method(mock,GetDeviceType)).AlwaysReturn(EDeviceType::EDeviceTypeRFID);
	When(Method(mock,GetModbusFunc)).AlwaysReturn(EModbusFunctions::EModbusReadHoldingRegisters);
	When(Method(mock,SetOffset).Using(offset)).AlwaysReturn();
	When(Method(mock,GetSizeInWords)).AlwaysReturn(50);
	When(Method(mock,Refresh)).AlwaysReturn();
}


ModbusObjectsMockGenerator::~ModbusObjectsMockGenerator() {
	// TODO Auto-generated destructor stub
}

}
