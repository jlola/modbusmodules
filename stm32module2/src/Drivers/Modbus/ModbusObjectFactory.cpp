/*
 * ModbusObjectFactory.cpp
 *
 *  Created on: 1. 12. 2017
 *      Author: Libor
 */

#ifndef UNITTEST

#include "stdlib.h"
#include <ModbusObjectFactory.h>
#include "OutputReg.h"
#include <Settings.h>

ModbusObjectFactory::ModbusObjectFactory(IModbusSlave * pslave,
		IUSART* rfidusart,
		ITimer* rfidtimer,
		OneWireManager* powmanager)
  : slave(pslave),
  owmanager(powmanager)
{
	modbusobjectsCount = 0;
	modbusobjects = NULL;
#ifdef RFID
	RFIDRegs* rfid = new RFIDRegs(rfidusart,slave,rfidtimer);
	IOPin* output1 = new IOPin(BIN_OUTPUT1_PORT,BIN_OUTPUT1_PIN,IOOutput,BIN_OUTPUT1_BOARD_PIN);
	IOPin* input1 = new IOPin(BIN_INPUT1_PORT,BIN_INPUT1_PIN,IOInput,BIN_INPUT1_BOARD_PIN);
	OutputReg* regoutput1 = new OutputReg(output1,pslave);
	InputReg* reginput1 = new InputReg(input1, slave);

	modbusobjectsCount = 4;
	modbusobjects = new IModbusObject*[modbusobjectsCount];
	modbusobjects[0] = dynamic_cast<IModbusObject*>(regoutput1);
	modbusobjects[1] = dynamic_cast<IModbusObject*>(reginput1);
	modbusobjects[2] = dynamic_cast<IModbusObject*>(owmanager);
	modbusobjects[3] = dynamic_cast<IModbusObject*>(rfid);
#endif
}

ModbusObjectFactory::ModbusObjectFactory(IModbusSlave * pslave,
		OneWireManager* powmanager)
  : slave(pslave), owmanager(powmanager)
{
	modbusobjectsCount = 0;
	modbusobjects = NULL;
#ifdef SMALL_SWITCH
	IOPin input1(BIN_INPUT1_PORT,BIN_INPUT1_PIN,IOInput,BIN_INPUT1_BOARD_PIN);
	IOPin input2(BIN_INPUT2_PORT,BIN_INPUT2_PIN,IOInput,BIN_INPUT2_BOARD_PIN);
	IOPin input3(BIN_INPUT3_PORT,BIN_INPUT3_PIN,IOInput,BIN_INPUT3_BOARD_PIN);
	IOPin output1(BIN_OUTPUT1_PORT,BIN_OUTPUT1_PIN,IOOutput,BIN_OUTPUT1_BOARD_PIN);
	InputReg reginput1(&input1, pslave);
	InputReg reginput2(&input2, pslave);
	InputReg reginput3(&input3, pslave);
	InputReg regoutput1(&output1,pslave);

	modbusobjectsCount = 5;
	modbusobjects = new IModbusObject*[modbusobjectsCount];

	modbusobjects[0] = dynamic_cast<IModbusObject*>(&reginput1);
	modbusobjects[1] = dynamic_cast<IModbusObject*>(&reginput2);
	modbusobjects[2] = dynamic_cast<IModbusObject*>(&reginput3);
	modbusobjects[3] = dynamic_cast<IModbusObject*>(&regoutput1);
	modbusobjects[4] = dynamic_cast<IModbusObject*>(owmanager);
#endif
}


size_t ModbusObjectFactory::ModbusObjectsCount()
{
	return modbusobjectsCount;
}

IModbusObject** ModbusObjectFactory::ModbusObjects()
{
	return modbusobjects;
}

ModbusObjectFactory::~ModbusObjectFactory()
{
}

#endif
