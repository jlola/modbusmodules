/*
 * Ds18b20Tests.cpp
 *
 *  Created on: 4. 12. 2017
 *      Author: Libor
 */

#include <Drivers/1wire/Ds18b20Tests.h>
#include "ds18B20.h"
namespace AF {

Ds18b20Tests::Ds18b20Tests() {
}

TEST_F(Ds18b20Tests,TemperatureConversionTest)
{
	ASSERT_EQ(25.06,(double)DS18B20::GetDouble(0x0191)/100);
	ASSERT_EQ(0.0,(double)DS18B20::GetDouble(0x0000)/100);
	ASSERT_EQ(-0.5,(double)DS18B20::GetDouble(0xFFF8)/100);
	ASSERT_EQ(-10.12,(double)DS18B20::GetDouble(0xFF5E)/100);
	ASSERT_EQ(-25.06,(double)DS18B20::GetDouble(0xFE6F)/100);
	ASSERT_EQ(-55,(double)DS18B20::GetDouble(0xFC90)/100);
}

Ds18b20Tests::~Ds18b20Tests() {
}

}
