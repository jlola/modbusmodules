#include "IOPin.h"
#include "stm32f0xx_GPIO.h"
#include "slave-rtu.h"
#include "DeviceFunctions.h"
#include "CFlash.h"




DeviceFunctions::DeviceFunctions()
: input1(BIN_INPUT1_PORT,BIN_INPUT1_PIN,IOInput),
  input2(BIN_INPUT2_PORT,BIN_INPUT2_PIN,IOInput),
  output1(BIN_OUTPUT1_PORT,BIN_OUTPUT1_PIN,IOOutput),
  output2(BIN_OUTPUT2_PORT,BIN_OUTPUT2_PIN,IOOutput)
{

}

void DeviceFunctions::Init(SlaveRtu* pslave,OneWireManager* owmanager)
{
	this->owmanager = owmanager;
	slave = pslave;

	input1.Init();
	input2.Init();
	output1.Init();
	output2.Init();

	slave->setHolding(COUNT_OF_TYPES_OFFSET,COUNT_OF_TYPES_VALUE);
	slave->setHolding(TYPE_DEFS_OFFSET,TYPE_DEFS_VALUE);

	slave->setHolding(INPUTS_BIN_TYPE,INPUTS_BIN_TYPE_VALUE);
	slave->setHolding(INPUTS_BIN_COUNT,INPUTS_BIN_COUNT_VALUE);
	slave->setHolding(INPUTS_BIN_ADDRESS,INPUTS_BIN_ADDRESS_VALUE);
	slave->setHolding(INPUTS_BIN_FUNC,INPUTS_BIN_FUNC_VALUE);

	slave->setHolding(OUTPUTS_BIN_TYPE,OUTPUTS_BIN_TYPE_VALUE);
	slave->setHolding(OUTPUTS_BIN_COUNT,OUTPUTS_BIN_COUNT_VALUE);
	slave->setHolding(OUTPUTS_BIN_ADDRESS,OUTPUTS_BIN_ADDRESS_VALUE);
	slave->setHolding(OUTPUTS_BIN_FUNC,OUTPUTS_BIN_FUNC_VALUE);

	slave->setHolding(OW_TYPE,OW_TYPE_VALUE);
	slave->setHolding(OW_COUNT,OW_COUNT_VALUE);
	slave->setHolding(OW_ADDRESS,OW_ADDRESS_VALUE);
	slave->setHolding(OW_FUNC,OW_FUNC_VALUE);

	inputs[0].Init(INPUTS_BIN_ADDRESS_VALUE+0,&input1,slave,BIN_INPUT1_BOARD_PIN);
	inputs[1].Init(INPUTS_BIN_ADDRESS_VALUE+1,&input2,slave,BIN_INPUT2_BOARD_PIN);

	outputs[0].Init(OUTPUTS_BIN_ADDRESS_VALUE+0,&output1,slave,BIN_OUTPUT1_BOARD_PIN);
	outputs[1].Init(OUTPUTS_BIN_ADDRESS_VALUE+1,&output2,slave,BIN_OUTPUT2_BOARD_PIN);

	writeresolvers[0] = &inputs[0];
	writeresolvers[1] = &inputs[1];
	writeresolvers[2] = &outputs[0];
	writeresolvers[3] = &outputs[1];
	writeresolvers[4] = owmanager;

	slave->init((IWriteReg**)writeresolvers,sizeof(writeresolvers)/sizeof(IWriteReg*));
}

void DeviceFunctions::Process()
{
	uint16_t csrH = RCC->CSR >> 16;
	slave->setHolding(RESET_REG_OFFSET,csrH);

	for (int i=0;i<INPUTS_BIN_COUNT_VALUE;i++)
	{
		inputs[i].Refresh();
	}

	for (int i=0;i<OUTPUTS_BIN_COUNT_VALUE;i++)
	{
		outputs[i].Refresh();
	}
}
