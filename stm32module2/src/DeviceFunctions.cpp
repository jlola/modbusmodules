
#include "Settings.h"
#include "TestDependedIncludes.h"
#include "DeviceFunctions.h"
#include "ModbusAddressManager.h"
#include "ModbusSettings.h"

DeviceFunctions::DeviceFunctions(IModbusSlave * slave, IModbusObjectFactory* factory)
{
	this->slave = slave;
	this->factory = factory;

	modbusobjects = factory->ModbusObjects();

	Init();
}



void DeviceFunctions::Init()
{
	slave->setHolding(TYPE_DEFS_OFFSET,TYPE_DEFS_START_ADDRESS);

	ModbusAddressManager addressmanager(slave,
			modbusobjects,
			factory->ModbusObjectsCount());
	addressmanager.ComputeAddresses();

	slave->init((IModbusObject**)modbusobjects,factory->ModbusObjectsCount());
}



void DeviceFunctions::Process()
{
#ifndef UNITTEST
	uint16_t csrH = RCC->CSR >> 16;
	slave->setHolding(RESET_REG_OFFSET,csrH);
#endif

	int size = factory->ModbusObjectsCount();

	for (int i=0;i<size;i++)
	{
		modbusobjects[i]->Refresh();
	}
}

