#ifndef _DEVICEFUNCTIONS_H_
#define _DEVICEFUNCTIONS_H_

#include "BinInputRegs.h"
#include "OutputReg.h"
#include "IModbusObject.h"
#include "AddressReg.h"

#define MAX_OBJECTS  5



class DeviceFunctions
{
	IModbusObject* modbusobjects[MAX_OBJECTS];
	IModbusSlave* slave;
	IWriteReg* owiremanager;
	//AddressReg baseregs;
	IOPin input1;
	IOPin input2;
	IOPin input3;
	IOPin output1;

	//IWriteReg* writeresolvers[INPUTS_BIN_COUNT_VALUE+OUTPUTS_BIN_COUNT_VALUE+1];
	InputReg reginput1;
	InputReg reginput2;
	InputReg reginput3;
	OutputReg regoutput1;


public:

	DeviceFunctions(IModbusSlave * pslave,IWriteReg* owiremanager);

	void Init();

	void Process();
};

#endif
