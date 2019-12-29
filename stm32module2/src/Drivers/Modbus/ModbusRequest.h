/*
 * ModbusRequest.h
 *
 *  Created on: 21. 10. 2019
 *      Author: pepa
 */

#ifndef DRIVERS_MODBUS_MODBUSREQUEST_H_
#define DRIVERS_MODBUS_MODBUSREQUEST_H_

#include <stdint.h>

typedef struct
{
	uint8_t Address;
	uint8_t Function;
	uint16_t StartingAddress;
	uint16_t Count;
	uint16_t ModbusCRC;
} ModbusRequest;


#endif /* DRIVERS_MODBUS_MODBUSREQUEST_H_ */
