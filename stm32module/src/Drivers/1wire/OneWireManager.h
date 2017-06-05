#ifndef _ONEWIREMANAGER_H_
#define _ONEWIREMANAGER_H_

#include "stdint.h"
#include "pt.h"
#include "ds18B20.h"
#include "slave-rtu.h"
#include "IWriteReg.h"

class OneWireManager : public IWriteReg
{
	SlaveRtu* slave;
	DS18B20 devices[DS18B20MAXCOUNT];
	OneWireThread* ow;
	pt ptScan;
	pt ptRefresh;

public:
	OneWireManager(OneWireThread* pow,SlaveRtu* slave);

	uint8_t GetCount();
	void SetCount(uint8_t count);

	void ModbusSetResetResult(EOWReset reset);
	uint8_t Scan();
	uint8_t Refresh();

	bool IsValid(uint16_t index, uint16_t reg);

	bool Write(uint16_t index, uint16_t reg);

	void StartScan();
};


#endif
