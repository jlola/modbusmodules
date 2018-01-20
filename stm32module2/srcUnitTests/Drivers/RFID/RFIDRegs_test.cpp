/*
 * RFIDRegs_test.cpp
 *
 *  Created on: 13. 11. 2017
 *      Author: Libor
 */

#include "RFIDRegs.h"
#include "RFIDRegs_test.h"
#include "IUSART.h"
#include "IUSARTHandler.h"
#include "IModbusSlave.h"
#include "ITimer.h"
#include "fakeit.hpp"

namespace AF {



void RFIDRegs_test::SetUp()
{
	When(Method(usartMock,SetHandler)).AlwaysReturn();
	When(Method(timerMock,IsStarted)).AlwaysReturn(true);
	When(Method(timerMock,Reset)).AlwaysReturn();
	When(Method(timerMock,Stop)).AlwaysReturn();
}


TEST_F(RFIDRegs_test,ConstructorSetUsartHandler)
{
	RFIDRegs regs(&usartMock.get(),&slaveMock.get(),&timerMock.get());
	Verify(Method(usartMock,SetHandler)).Once();
}

TEST_F(RFIDRegs_test,IsValid_SetCorrectValueReturnsTrue)
{
	RFIDRegs regs(&usartMock.get(),&slaveMock.get(),&timerMock.get());
	regs.SetOffset(offset);
	ASSERT_TRUE(regs.IsValid(offset,0));
}

TEST_F(RFIDRegs_test,IsValid_SetInCorrectValueReturnsFalse)
{

	RFIDRegs regs(&usartMock.get(),&slaveMock.get(),&timerMock.get());
	regs.SetOffset(offset);
	ASSERT_FALSE(regs.IsValid(11,0));
}

TEST_F(RFIDRegs_test,Write_RecDataSetNewDataFlag)
{
	When(Method(slaveMock,setHolding).Using(offset,0)).AlwaysReturn();
	RFIDRegs regs(&usartMock.get(),&slaveMock.get(),&timerMock.get());
	regs.OnReceiveData(recchar);
	regs.OnHWTimer(1);
	ASSERT_EQ(regs.regs.NewDataFlag,1);
}

TEST_F(RFIDRegs_test,Write_RecDataThenResetDataFlag)
{

	When(Method(slaveMock,setHolding).Using(offset,0)).AlwaysReturn();
	RFIDRegs regs(&usartMock.get(),&slaveMock.get(),&timerMock.get());
	regs.SetOffset(offset);
	regs.OnReceiveData(recchar);
	regs.OnHWTimer(1);
	When(Method(slaveMock,setHoldings).Using(10,(uint16_t*)&regs.regs,sizeof(SRFIDRegs)/2)).AlwaysReturn();
	regs.Write(offset,1);
	regs.Refresh();
	ASSERT_EQ(regs.regs.NewDataFlag,0);
}

TEST_F(RFIDRegs_test,Write_RecDataStoredToSlave)
{
	When(Method(slaveMock,setHolding).Using(offset,0)).AlwaysReturn();
	RFIDRegs regs(&usartMock.get(),&slaveMock.get(),&timerMock.get());
	regs.SetOffset(offset);
	When(Method(slaveMock,setHoldings).Using(offset,(uint16_t*)&regs.regs,sizeof(SRFIDRegs)/2)).AlwaysReturn();
	regs.OnReceiveData(recchar);
	regs.OnHWTimer(1);
	regs.Refresh();
	When(Method(slaveMock,getHolding).Using(offset+1)).AlwaysReturn(((uint16_t*)&regs.regs)[1]);
	ASSERT_EQ(recchar,slaveMock.get().getHolding(offset+1));
}

}
