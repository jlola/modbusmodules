#include "string.h"
#include "OneWireThread.h"
#include "OneWireManager.h"


OneWireManager::OneWireManager(OneWireThread* pow,SlaveRtu* Slave)
	: ow(pow),slave(Slave)
{
	PT_INIT(&ptRefresh);
	PT_INIT(&ptScan);

	SetCount(0);
}

bool OneWireManager::IsValid(uint16_t index, uint16_t reg)
{
	switch(index)
	{
		case OW_SCAN_OFFSET:
			return true;
		default:
			return false;
	}
}

void OneWireManager::StartScan()
{
	Write(OW_SCAN_OFFSET,1);
}

bool OneWireManager::Write(uint16_t index, uint16_t reg)
{
	if (index==OW_SCAN_OFFSET && reg > 0)
	{
		slave->setHolding(OW_SCAN_OFFSET,1);
	}
}

uint8_t OneWireManager::GetCount()
{
	return slave->getHolding(OW_COUNT_OFFSET);
}

void OneWireManager::SetCount(uint8_t count)
{
	slave->setHolding(OW_COUNT_OFFSET,count);
	slave->setHolding(LAST_INDEX,OW_DEVICES_OFFSET+DS18B20_SIZE*count);
}

void OneWireManager::ModbusSetResetResult(EOWReset reset)
{
	slave->setHolding(OW_RESET_RESULT,reset);
}

uint8_t OneWireManager::Refresh()
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
			uint8_t offset = OW_DEVICES_OFFSET + owcount * DS18B20::Size;
			devices[owcount].Init(ow,addr,slave,offset);
			owcount++;
		}
	} while(result);

	SetCount(owcount);


	PT_END(&ptScan);
}
