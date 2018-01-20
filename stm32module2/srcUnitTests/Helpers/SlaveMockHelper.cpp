/*
 * SlaveMock.cpp
 *
 *  Created on: 3. 12. 2017
 *      Author: Libor
 */

#include <SlaveMockHelper.h>
#include "Settings.h"
#include "fakeit.hpp"

using namespace fakeit;

SlaveMockHelper::SlaveMockHelper() {
	// TODO Auto-generated constructor stub

}

Mock<IModbusSlave> & SlaveMockHelper::GetSlaveMock()
{
	const int COUNT_OF_TYPES = 4;
	const int INPUT_TYPE = EDeviceTypeBinInputs;
	const int INPUT_START = 27;
	const int INPUT_COUNT = 3;
	const int INPUT_READ_FUNC = EModbusReadHoldingRegisters;

	const int OUTPUT_TYPE = EDeviceTypeBinOutputs;
	const int OUTPUT_COUNT = 1;
	const int OUTPUT_START = 42;
	const int OUTPUT_READ_FUNC = EModbusReadHoldingRegisters;

	const int DS18B20_TYPE = EDeviceTypeDS18B20;
	const int DS18B20_COUNT = 1;
	const int DS18B20_START = 45;
	const int DS18B20_READ_FUNC = EModbusReadHoldingRegisters;

	const int RFID_TYPE = EDeviceTypeRFID;
	const int RFID_COUNT = 1;
	const int RFID_START = 105;
	const int RFID_READ_FUNC = EModbusReadHoldingRegisters;

	const int LAST_INDEX_VALUE = 155;

	When(Method(mockModbusSlave,GetMaxIndex)).AlwaysReturn(SLAVERTU_HOLDINGS);
	When(Method(mockModbusSlave,setHolding).Using(1,COUNT_OF_TYPES)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(LAST_INDEX, LAST_INDEX_VALUE)).AlwaysReturn();

	When(Method(mockModbusSlave,setHolding).Using(10,INPUT_TYPE)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(11,INPUT_COUNT)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(12,INPUT_START)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(13,INPUT_READ_FUNC)).AlwaysReturn();

	When(Method(mockModbusSlave,setHolding).Using(14,OUTPUT_TYPE)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(15,OUTPUT_COUNT)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(16,OUTPUT_START)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(17,OUTPUT_READ_FUNC)).AlwaysReturn();

	When(Method(mockModbusSlave,setHolding).Using(18,DS18B20_TYPE)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(19,DS18B20_COUNT)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(20,DS18B20_START)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(21,DS18B20_READ_FUNC)).AlwaysReturn();

	When(Method(mockModbusSlave,setHolding).Using(22,RFID_TYPE)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(23,RFID_COUNT)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(24,RFID_START)).AlwaysReturn();
	When(Method(mockModbusSlave,setHolding).Using(25,RFID_READ_FUNC)).AlwaysReturn();
	return mockModbusSlave;

}

SlaveMockHelper::~SlaveMockHelper() {
	// TODO Auto-generated destructor stub
}

