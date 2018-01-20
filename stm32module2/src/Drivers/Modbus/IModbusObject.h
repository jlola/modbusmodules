/*
 * IModbusType.h
 *
 *  Created on: 26. 9. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_MODBUS_IMODBUSOBJECT_H_
#define DRIVERS_MODBUS_IMODBUSOBJECT_H_

//	slave->setHolding(INPUTS_BIN_TYPE,INPUTS_BIN_TYPE_VALUE);
//	slave->setHolding(INPUTS_BIN_COUNT,INPUTS_BIN_COUNT_VALUE);
//	slave->setHolding(INPUTS_BIN_ADDRESS,INPUTS_BIN_ADDRESS_VALUE);
//	slave->setHolding(INPUTS_BIN_FUNC,INPUTS_BIN_FUNC_VALUE);

#include "ModbusSettings.h"
#include "Settings.h"
#include "stdint.h"

class IModbusObject
{
public:
	virtual bool IsValid(uint16_t index, uint16_t reg)=0;
	virtual bool Write(uint16_t index, uint16_t reg)=0;
	virtual void Refresh()=0;
	virtual EDeviceType GetDeviceType()=0;
	virtual EModbusFunctions GetModbusFunc()=0;
	virtual uint16_t GetSizeInWords()=0;
	virtual void SetOffset(uint16_t offset)=0;
	virtual ~IModbusObject(){}
};


#endif /* DRIVERS_MODBUS_IMODBUSOBJECT_H_ */
