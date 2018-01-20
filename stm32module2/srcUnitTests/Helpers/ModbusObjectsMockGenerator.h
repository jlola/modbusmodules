/*
 * ModbusObjectsMockGenerator.h
 *
 *  Created on: 2. 12. 2017
 *      Author: Libor
 */

#ifndef HELPERS_MODBUSOBJECTSMOCKGENERATOR_H_
#define HELPERS_MODBUSOBJECTSMOCKGENERATOR_H_

#include "IModbusObject.h"
#include "Settings.h"

#include "gtest/gtest.h"
#include "fakeit.hpp"

namespace AF {

using namespace fakeit;

class ModbusObjectsMockGenerator {
	void CreateInput(Mock<IModbusObject> & inputmock,uint16_t offset);
	void CreateOutput(Mock<IModbusObject> & outputmock,uint16_t offset);
	Mock<IModbusObject> &  CreateDS18B20(Mock<IModbusObject> & mock,uint16_t offset);
	void CreateRFIDRegs(Mock<IModbusObject> & mock,uint16_t offset);
public:
	ModbusObjectsMockGenerator();


	void CreateInput1(Mock<IModbusObject> & inputmock);
	void CreateInput2(Mock<IModbusObject> & inputmock);
	void CreateInput3(Mock<IModbusObject> & inputmock);
	void CreateOutput1(Mock<IModbusObject> & outputmock);
	void CreateDS18B20(Mock<IModbusObject> & ds18b20mock);
	void CreateRFIDRegs(Mock<IModbusObject> & mock);

	virtual ~ModbusObjectsMockGenerator();

	const int OFFSET_INPUT1 = 27;
	const int OFFSET_INPUT2 = 32;
	const int OFFSET_INPUT3 = 37;
	const int OFFSET_OUTPUT1 = 42;
	const int OFFSET_DS18B20 = 45;
	const int OFFSET_RFIDREG = 105;

};

}

#endif /* HELPERS_MODBUSOBJECTSMOCKGENERATOR_H_ */
