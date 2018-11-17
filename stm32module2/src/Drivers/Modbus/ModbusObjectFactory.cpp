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
	IOPin* input1 = new IOPin(BIN_INPUT1_PORT,BIN_INPUT1_PIN,IOInput,BIN_INPUT1_BOARD_PIN);
	IOPin* output1 = new IOPin(BIN_OUTPUT1_PORT,BIN_OUTPUT1_PIN,IOOutput,BIN_OUTPUT1_BOARD_PIN);
	InputReg* reginput1 = new InputReg(input1, pslave);
	OutputReg* regoutput1 = new OutputReg(output1,pslave);
#ifndef SMALL_SWITCH_1INPUT
	IOPin* input2 = new IOPin(BIN_INPUT2_PORT,BIN_INPUT2_PIN,IOInput,BIN_INPUT2_BOARD_PIN);
	IOPin* input3 = new IOPin(BIN_INPUT3_PORT,BIN_INPUT3_PIN,IOInput,BIN_INPUT3_BOARD_PIN);
	InputReg* reginput2 = new InputReg(input2, pslave);
	InputReg* reginput3 = new InputReg(input3, pslave);

	modbusobjectsCount = 5;
	modbusobjects = new IModbusObject*[modbusobjectsCount];

	modbusobjects[0] = dynamic_cast<IModbusObject*>(reginput1);
	modbusobjects[1] = dynamic_cast<IModbusObject*>(reginput2);
	modbusobjects[2] = dynamic_cast<IModbusObject*>(reginput3);
	modbusobjects[3] = dynamic_cast<IModbusObject*>(regoutput1);
	modbusobjects[4] = dynamic_cast<IModbusObject*>(owmanager);
#endif
#elif defined BIGRELIO
	IOPin* input1 = new IOPin(BIN_INPUT1_PORT,BIN_INPUT1_PIN,IOInput,BIN_INPUT1_BOARD_PIN);
	IOPin* input2 = new IOPin(BIN_INPUT2_PORT,BIN_INPUT2_PIN,IOInput,BIN_INPUT2_BOARD_PIN);
	IOPin* input3 = new IOPin(BIN_INPUT3_PORT,BIN_INPUT3_PIN,IOInput,BIN_INPUT3_BOARD_PIN);
	IOPin* input4 = new IOPin(BIN_INPUT4_PORT,BIN_INPUT4_PIN,IOInput,BIN_INPUT4_BOARD_PIN);
	IOPin* input5 = new IOPin(BIN_INPUT5_PORT,BIN_INPUT5_PIN,IOInput,BIN_INPUT5_BOARD_PIN);
	IOPin* input6 = new IOPin(BIN_INPUT6_PORT,BIN_INPUT6_PIN,IOInput,BIN_INPUT6_BOARD_PIN);

	IOPin* output7 = new IOPin(BIN_OUTPUT7_PORT,BIN_OUTPUT7_PIN,IOOutput,BIN_OUTPUT7_BOARD_PIN);
	IOPin* output8 = new IOPin(BIN_OUTPUT8_PORT,BIN_OUTPUT8_PIN,IOOutput,BIN_OUTPUT8_BOARD_PIN);
	IOPin* output9 = new IOPin(BIN_OUTPUT9_PORT,BIN_OUTPUT9_PIN,IOOutput,BIN_OUTPUT9_BOARD_PIN);
	IOPin* output10 = new IOPin(BIN_OUTPUT10_PORT,BIN_OUTPUT10_PIN,IOOutput,BIN_OUTPUT10_BOARD_PIN);
	IOPin* output11 = new IOPin(BIN_OUTPUT11_PORT,BIN_OUTPUT11_PIN,IOOutput,BIN_OUTPUT11_BOARD_PIN);
	IOPin* output12 = new IOPin(BIN_OUTPUT12_PORT,BIN_OUTPUT12_PIN,IOOutput,BIN_OUTPUT12_BOARD_PIN);
	IOPin* output13 = new IOPin(BIN_OUTPUT13_PORT,BIN_OUTPUT13_PIN,IOOutput,BIN_OUTPUT13_BOARD_PIN);
	IOPin* output14 = new IOPin(BIN_OUTPUT14_PORT,BIN_OUTPUT14_PIN,IOOutput,BIN_OUTPUT14_BOARD_PIN);
	IOPin* output15 = new IOPin(BIN_OUTPUT15_PORT,BIN_OUTPUT15_PIN,IOOutput,BIN_OUTPUT15_BOARD_PIN);

	InputReg* reginput1 = new InputReg(input1, pslave);
	InputReg* reginput2 = new InputReg(input2, pslave);
	InputReg* reginput3 = new InputReg(input3, pslave);
	InputReg* reginput4 = new InputReg(input4, pslave);
	InputReg* reginput5 = new InputReg(input5, pslave);
	InputReg* reginput6 = new InputReg(input6, pslave);

	OutputReg* regoutput7 = new OutputReg(output7,pslave);
	OutputReg* regoutput8 = new OutputReg(output8,pslave);
	OutputReg* regoutput9 = new OutputReg(output9,pslave);
	OutputReg* regoutput10 = new OutputReg(output10,pslave);
	OutputReg* regoutput11 = new OutputReg(output11,pslave);
	OutputReg* regoutput12 = new OutputReg(output12,pslave);
	OutputReg* regoutput13 = new OutputReg(output13,pslave);
	OutputReg* regoutput14 = new OutputReg(output14,pslave);
	OutputReg* regoutput15 = new OutputReg(output15,pslave);

	modbusobjectsCount = 16;
	modbusobjects = new IModbusObject*[modbusobjectsCount];

	modbusobjects[0] = dynamic_cast<IModbusObject*>(reginput1);
	modbusobjects[1] = dynamic_cast<IModbusObject*>(reginput2);
	modbusobjects[2] = dynamic_cast<IModbusObject*>(reginput3);
	modbusobjects[3] = dynamic_cast<IModbusObject*>(reginput4);
	modbusobjects[4] = dynamic_cast<IModbusObject*>(reginput5);
	modbusobjects[5] = dynamic_cast<IModbusObject*>(reginput6);

	modbusobjects[6] = dynamic_cast<IModbusObject*>(regoutput7);
	modbusobjects[7] = dynamic_cast<IModbusObject*>(regoutput8);
	modbusobjects[8] = dynamic_cast<IModbusObject*>(regoutput9);
	modbusobjects[9] = dynamic_cast<IModbusObject*>(regoutput10);
	modbusobjects[10] = dynamic_cast<IModbusObject*>(regoutput11);
	modbusobjects[11] = dynamic_cast<IModbusObject*>(regoutput12);
	modbusobjects[12] = dynamic_cast<IModbusObject*>(regoutput13);
	modbusobjects[13] = dynamic_cast<IModbusObject*>(regoutput14);
	modbusobjects[14] = dynamic_cast<IModbusObject*>(regoutput15);
	modbusobjects[15] = dynamic_cast<IModbusObject*>(owmanager);

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
