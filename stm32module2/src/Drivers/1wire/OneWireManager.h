#ifndef _ONEWIREMANAGER_H_
#define _ONEWIREMANAGER_H_

#include "stdint.h"
#include "pt.h"
#include "ds18B20.h"
#include "IModbusSlave.h"
#include "IWriteReg.h"
#include "IOneWireThread.h"
#include "IModbusObject.h"

#define DS18B20MAXCOUNT 			10
#define OW_ADDRESS_VALUE offset
#define OW_SCAN_OFFSET				OW_ADDRESS_VALUE+0
#define OW_COUNT_OFFSET 			OW_ADDRESS_VALUE+1
#define OW_BOARD_PIN				OW_ADDRESS_VALUE+2
#define OW_RESET_RESULT				OW_ADDRESS_VALUE+3

#define OW_DEVICES_OFFSET 			OW_ADDRESS_VALUE+5

class OneWireManager : public IWriteReg
{
	IOneWireThread* ow;
	IModbusSlave* slave;
	DS18B20 devices[DS18B20MAXCOUNT];
	pt ptScan;
	pt ptRefresh;
	uint16_t offset;

	uint8_t RefreshThread();
public:
	OneWireManager(IOneWireThread* pow,IModbusSlave* slave);

    void SetOffset(uint16_t offset);

	uint8_t GetCount();
	void SetCount(uint8_t count);

	void ModbusSetResetResult(EOWReset reset);
	uint8_t Scan();
	void Refresh();

	bool IsValid(uint16_t index, uint16_t reg);

	bool Write(uint16_t index, uint16_t reg);

	void StartScan();

	EDeviceType GetDeviceType();
	EModbusFunctions GetModbusFunc();
	uint16_t GetSizeInWords();

};


#endif
