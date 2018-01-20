/*
 * DeviceFunctionsTest.h
 *
 *  Created on: 2. 12. 2017
 *      Author: Libor
 */

#ifndef UNITTESTS_DEVICEFUNCTIONSTEST_H_
#define UNITTESTS_DEVICEFUNCTIONSTEST_H_

#include "gtest/gtest.h"
#include "fakeit.hpp"


namespace AF {

class DeviceFunctionsTest : public testing::Test {
public:
	DeviceFunctionsTest();
	virtual ~DeviceFunctionsTest();
};


}

#endif /* UNITTESTS_DEVICEFUNCTIONSTEST_H_ */
