/*
 * SlaveMock.h
 *
 *  Created on: 3. 12. 2017
 *      Author: Libor
 */

#ifndef HELPERS_SLAVEMOCKHELPER_H_
#define HELPERS_SLAVEMOCKHELPER_H_

#include "gtest/gtest.h"
#include "fakeit.hpp"
#include "IModbusSlave.h"

using namespace fakeit;

class SlaveMockHelper {
	Mock<IModbusSlave> mockModbusSlave;
public:
	SlaveMockHelper();
	Mock<IModbusSlave> & GetSlaveMock();
	virtual ~SlaveMockHelper();
};

#endif /* HELPERS_SLAVEMOCKHELPER_H_ */
