#ifndef _DEVICEFUNCTIONS_H_
#define _DEVICEFUNCTIONS_H_

#include "slave-rtu.h"
#include "InputReg.h"
#include "OutputReg.h"

class DeviceFunctions
{
	IOPin input1;
	IOPin input2;
	IOPin input3;
	IOPin output1;
	SlaveRtu* slave;
	IWriteReg* owiremanager;

	IWriteReg* writeresolvers[INPUTS_BIN_COUNT_VALUE+OUTPUTS_BIN_COUNT_VALUE+1];

	InputReg inputs[INPUTS_BIN_COUNT_VALUE];
	OutputReg outputs[OUTPUTS_BIN_COUNT_VALUE];

public:

	DeviceFunctions();

	void Init(SlaveRtu* pslave,IWriteReg* owmanager);

	void Process();
};

#endif
