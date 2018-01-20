/*
 * ModbusAddressManager.cpp
 *
 *  Created on: 31. 10. 2017
 *      Author: Libor
 */
#include <stddef.h>
#include <assert.h>

#ifdef UNITTEST
#include "TestSettings.h"
#include "AssertException.h"
#endif

#include "Settings.h"
#include "ModbusAddressManager.h"

ModbusAddressManager::ModbusAddressManager(IModbusSlave * slave,IModbusObject* modbusobjects[],size_t count) {
	assert_param(slave!=NULL);
	// TODO Auto-generated constructor stub
	this->slave = slave;
	this->modbusobjects = modbusobjects;
	this->objectsCount = count;
}

void ModbusAddressManager::ComputeAddresses()
{
	uint8_t typesCounts = GetObjectTypeCounts();
	//slave.setHolding(COUNT_OF_TYPES_OFFSET,COUNT_OF_TYPES_VALUE);
	slave->setHolding(COUNT_OF_TYPES_OFFSET,typesCounts);
	//count of types
	int typeInfoLength = sizeof(STypeInfo) / 2;
	int allTypesLength = typeInfoLength*typesCounts;

	EDeviceType type = EDeviceTypeNone;
	uint8_t typeInfoIndex = 0;
	uint8_t typeObjectsCounts = 0;
	uint16_t instancesSize = 0;
	uint16_t startAddressOfInstances = TYPE_DEFS_START_ADDRESS + allTypesLength + 1;

	for(uint8_t i=0;i<(uint8_t)objectsCount;i++)
	{
		typeObjectsCounts++;
		type = modbusobjects[i]->GetDeviceType();
		modbusobjects[i]->SetOffset(startAddressOfInstances+instancesSize);
		instancesSize += modbusobjects[i]->GetSizeInWords();

		if ( i < objectsCount - 1 )
		{
			if (modbusobjects[i+1]->GetDeviceType()==type)
			{
				continue;
			}
		}

		STypeInfo objType;
		objType.Address = startAddressOfInstances;
		objType.Count = typeObjectsCounts;
		objType.ReadFunc = EModbusReadHoldingRegisters;
		objType.Type = type;
		WriteTypeInfo(TYPE_DEFS_START_ADDRESS+typeInfoIndex*typeInfoLength, objType);
		typeObjectsCounts = 0;

		typeInfoIndex++;
		startAddressOfInstances += instancesSize;
		instancesSize = 0;
	}

	assert_param(startAddressOfInstances<slave->GetMaxIndex());
	slave->setHolding(LAST_INDEX,startAddressOfInstances+instancesSize);
}

uint8_t ModbusAddressManager::GetObjectTypeCounts()
{
	uint8_t typeCounts = 0;
	for(uint8_t i=0;i<objectsCount;i++)
	{
		typeCounts++;
		EDeviceType type = modbusobjects[i]->GetDeviceType();

		while(i<objectsCount-1 && modbusobjects[i+1]->GetDeviceType() == type)
		{
			i++;
		}
	}
	return typeCounts;
}

void ModbusAddressManager::WriteTypeInfo(uint8_t addr, STypeInfo type)
{
	slave->setHolding(addr, type.Type);
	slave->setHolding(addr+1, type.Count);
	slave->setHolding(addr+2, type.Address);
	slave->setHolding(addr+3, type.ReadFunc);
}

ModbusAddressManager::~ModbusAddressManager() {
	// TODO Auto-generated destructor stub
}

