#ifndef _DEVICEFUNCTIONS_H_
#define _DEVICEFUNCTIONS_H_

#include "slave-rtu.h"
#include "InputReg.h"
#include "OutputReg.h"
#include "OneWireManager.h"

class DeviceFunctions
{
	OneWireManager* owmanager;
	IOPin input1;
	IOPin input2;
	IOPin output1;
	IOPin output2;
	SlaveRtu* slave;

	IWriteReg* writeresolvers[INPUTS_BIN_COUNT_VALUE+OUTPUTS_BIN_COUNT_VALUE+1];

	InputReg inputs[INPUTS_BIN_COUNT_VALUE];
	OutputReg outputs[OUTPUTS_BIN_COUNT_VALUE];

public:

	DeviceFunctions();

	void Init(SlaveRtu* pslave,OneWireManager* owmanager);

	void Process();
};

#endif
