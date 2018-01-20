/*
 * RFIDRegs_test.h
 *
 *  Created on: 3. 12. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_RFID_RFIDREGS_TEST_H_
#define DRIVERS_RFID_RFIDREGS_TEST_H_

#include "gtest/gtest.h"
#include "fakeit.hpp"

namespace AF {

using namespace fakeit;

class RFIDRegs_test : public testing::Test
{
public:
	const char recchar = 'd';
	const int offset = 10;
	Mock<IUSART> usartMock;
	Mock<IUSARTHandler> usartHandlerMock;
	Mock<IModbusSlave> slaveMock;
	Mock<ITimer> timerMock;

	void SetUp();
};

}

#endif /* DRIVERS_RFID_RFIDREGS_TEST_H_ */
