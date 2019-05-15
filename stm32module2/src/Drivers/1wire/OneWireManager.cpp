#include "string.h"
#include "OneWireManager.h"
#include "TestDependedIncludes.h"

#ifdef UNITTEST
#include "TestSettings.h"
#endif

OneWireManager::OneWireManager(IOneWireThread* pow,IModbusSlave* Slave)
	: ow(pow),slave(Slave)
{
	offset = 0;
	PT_INIT(&ptRefresh);
	PT_INIT(&ptScan);

}

void OneWireManager::SetOffset(uint16_t offset)
{
	this->offset = offset;
	SetCount(0);
}

bool OneWireManager::IsValid(uint16_t index, uint16_t reg)
{
	if (OW_SCAN_OFFSET==index)
		return true;
	return false;
}

void OneWireManager::StartScan()
{
	assert_param( offset > 0 );
	Write(OW_SCAN_OFFSET,1);
}

bool OneWireManager::Write(uint16_t index, uint16_t reg)
{
	assert_param(offset>0);
	if (index==OW_SCAN_OFFSET && reg > 0)
	{
		slave->setHolding(OW_SCAN_OFFSET,1,false);
		return true;
	}
	return false;
}

uint8_t OneWireManager::GetCount()
{
	assert_param(offset>0);
	return slave->getHolding(OW_COUNT_OFFSET);
}

void OneWireManager::SetCount(uint8_t count)
{
	assert_param(offset>0);
	slave->setHolding(OW_COUNT_OFFSET,count,true);
}

void OneWireManager::ModbusSetResetResult(EOWReset reset)
{
	assert_param(offset>0);
	slave->setHolding(OW_RESET_RESULT,reset,true);
}

bool OneWireManager::Refresh()
{
	assert_param(offset>0);
	RefreshThread();
}

uint8_t OneWireManager::RefreshThread()
{
	static uint8_t count;
	static uint8_t indexRefresh;

	PT_BEGIN(&ptRefresh);
	count = GetCount();

	for(indexRefresh=0;indexRefresh<count;indexRefresh++)
	{
		PT_WAIT_THREAD(&ptRefresh,devices[indexRefresh].Read());
	}

	if (slave->getHolding(OW_SCAN_OFFSET))
	{
		slave->setHolding(OW_SCAN_OFFSET,0,false);
		PT_WAIT_THREAD(&ptRefresh,Scan());
	}

	PT_END(&ptRefresh);
}

uint8_t OneWireManager::Scan()
{
	assert_param(offset>0);
	uint8_t result;
	static uint8_t owcount;
	result = 0;
	PT_BEGIN(&ptScan);
	owcount = 0;
	do {
		uint8_t addr[8];
		PT_WAIT_THREAD(&ptScan,ow->OWSearch(addr,result));
		if (result)
		{
			uint8_t owoffset = OW_DEVICES_OFFSET + owcount * DS18B20::Size;
			devices[owcount].Init(ow,addr,slave,owoffset);
			owcount++;
		}
	} while(result);

	SetCount(owcount);


	PT_END(&ptScan);
}

EDeviceType OneWireManager::GetDeviceType()
{
	return EDeviceType::EDeviceTypeDS18B20;
}
EModbusFunctions OneWireManager::GetModbusFunc()
{
	return EModbusFunctions::EModbusReadHoldingRegisters;
}
uint16_t OneWireManager::GetSizeInWords()
{
	return 5+DS18B20::Size*DS18B20MAXCOUNT;
}

