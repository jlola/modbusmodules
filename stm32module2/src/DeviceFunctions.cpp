#ifndef UNITTEST

#include "stm32f0xx_GPIO.h"
#include "slave-rtu.h"
#include "CFlash.h"

#include "IOPin.h"
#include "Settings.h"
#include "DeviceFunctions.h"
#include "ModbusAddressManager.h"

DeviceFunctions::DeviceFunctions(IModbusSlave * pslave, IWriteReg* owiremanager)
: slave(pslave),owiremanager(NULL),//baseregs(pslave),
  input1(BIN_INPUT1_PORT,BIN_INPUT1_PIN,IOInput,BIN_INPUT1_BOARD_PIN),
  input2(BIN_INPUT2_PORT,BIN_INPUT2_PIN,IOInput,BIN_INPUT2_BOARD_PIN),
  input3(BIN_INPUT3_PORT,BIN_INPUT3_PIN,IOInput,BIN_INPUT3_BOARD_PIN),
  output1(BIN_OUTPUT1_PORT,BIN_OUTPUT1_PIN,IOOutput,BIN_OUTPUT1_BOARD_PIN),
  reginput1(&input1, slave),
  reginput2(&input2, slave),
  reginput3(&input3, slave),
  regoutput1(&output1,slave)

{
	this->owiremanager = owiremanager;
	//modbusobjects[0] = dynamic_cast<IModbusObject*>(&baseregs);
	modbusobjects[0] = dynamic_cast<IModbusObject*>(&reginput1);
	modbusobjects[1] = dynamic_cast<IModbusObject*>(&reginput2);
	modbusobjects[2] = dynamic_cast<IModbusObject*>(&reginput3);
	modbusobjects[3] = dynamic_cast<IModbusObject*>(&regoutput1);
	modbusobjects[4] = dynamic_cast<IModbusObject*>(owiremanager);
}



void DeviceFunctions::Init()
{
	slave->setHolding(TYPE_DEFS_OFFSET,TYPE_DEFS_START_ADDRESS);

	ModbusAddressManager addressmanager(slave,modbusobjects,sizeof(modbusobjects)/sizeof(IModbusObject*));
	addressmanager.ComputeAddresses();

	slave->init((IModbusObject**)modbusobjects,sizeof(modbusobjects) / sizeof(IModbusObject*));
}



void DeviceFunctions::Process()
{
	uint16_t csrH = RCC->CSR >> 16;
	slave->setHolding(RESET_REG_OFFSET,csrH);

	int size = sizeof(modbusobjects) / sizeof(IModbusObject*);

	for (int i=0;i<size;i++)
	{
		modbusobjects[i]->Refresh();
	}
}

#endif
