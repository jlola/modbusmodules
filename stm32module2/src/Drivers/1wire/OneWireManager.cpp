#ifndef UNITTEST

#include "string.h"
#include "OneWireThread.h"
#include "OneWireManager.h"


OneWireManager::OneWireManager(OneWireThread* pow,SlaveRtu* Slave)
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
		slave->setHolding(OW_SCAN_OFFSET,1);
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
	slave->setHolding(OW_COUNT_OFFSET,count);
}

void OneWireManager::ModbusSetResetResult(EOWReset reset)
{
	assert_param(offset>0);
	slave->setHolding(OW_RESET_RESULT,reset);
}

void OneWireManager::Refresh()
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

	EOWReset reset;
	PT_WAIT_THREAD(&ptRefresh,ow->OWReset(reset));
	ModbusSetResetResult(reset);

	if (reset == EOWResetPresent)
	{
		for(indexRefresh=0;indexRefresh<count;indexRefresh++)
		{
			PT_WAIT_THREAD(&ptRefresh,devices[indexRefresh].Read());
		}

		if (slave->getHolding(OW_SCAN_OFFSET))
		{
			slave->setHolding(OW_SCAN_OFFSET,0);
			PT_WAIT_THREAD(&ptRefresh,Scan());
		}
	}
	else {
		slave->setHolding(OW_SCAN_OFFSET,0);
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

#endif
