/*
 * Ds18b20Tests.h
 *
 *  Created on: 4. 12. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_1WIRE_DS18B20TESTS_H_
#define DRIVERS_1WIRE_DS18B20TESTS_H_

#include "gtest/gtest.h"
#include "fakeit.hpp"

namespace AF {

class Ds18b20Tests  : public testing::Test
{
public:
	Ds18b20Tests();
	virtual ~Ds18b20Tests();
};

}

#endif /* DRIVERS_1WIRE_DS18B20TESTS_H_ */
