#include "ds18B20.h"
#include "pt.h"
#include "string.h"
#include "diag/Trace.h"
#include "CString.h"


DS18B20::DS18B20()
{
	PT_INIT(&ptDS18b20);
	baseAddr = 0;
	slave = NULL;
	ow = NULL;
}

//when is error there is written 0 otherwise 1
uint16_t DS18B20::GetError()
{
	return slave->getHolding(baseAddr+DS18B20_ERROR) > 0 ? 0 : 1;
}
//when is error there is written 0 otherwise 1
void DS18B20::SetError(uint16_t error)
{
	slave->setHolding(baseAddr+DS18B20_ERROR,error>0?0:1);
}

uint16_t DS18B20::GetTemp()
{
	return slave->getHolding(baseAddr+DS18B20_TEMPER);
}
bool DS18B20::SetTemp(uint16_t temp)
{
	if (GetTemp()!=temp)
	{
		slave->setHolding(baseAddr+DS18B20_TEMPER,temp);
		return true;
	}
	return false;
}


void DS18B20::Init(IOneWireThread* owthread,uint8_t * owAddr,IModbusSlave* Slave, uint8_t BaseAddr)
{
	baseAddr = BaseAddr;
	slave = Slave;
	ow = owthread;
	uint16_t* ow16Addr = (uint16_t*)owAddr;
	for(uint8_t i=0;i<8;i++)
		addr[i] = owAddr[i];
	for(uint8_t i=0;i<4;i++)
	{
		slave->setHolding(baseAddr+i,ow16Addr[i]);
	}
}

int16_t DS18B20::GetDouble(uint16_t pValue)
	{
		uint8_t signedflag = pValue >> 12;

		if (signedflag)
			pValue = 0xFFFF - pValue + 1;

		int16_t result = pValue >>4;
		result *= 100;

		if (pValue & 0x08) result += 50;
		if (pValue & 0x04) result += 25;
		if (pValue & 0x02) result += 12;
		if (pValue & 0x01) result += 6;

		if (signedflag) result = result*-1;

		return result;
	}

uint8_t error;

uint8_t DS18B20::Read()
{
	PT_BEGIN(&ptDS18b20);
	//PT_WAIT_THREAD(&ptDS18b20 ,OWReset(reset));
	//PT_WAIT_THREAD(&ptDS18b20 ,OWSearch(newaddr,result));

	static char bytes[sizeof(scratchpadDS1820_t)];

	EOWReset reset;
	PT_WAIT_THREAD(&ptDS18b20 ,ow->OWReset(reset));

	if (reset == EOWResetPresent)
	{
		//match rom
		PT_WAIT_THREAD(&ptDS18b20 ,ow->OWSelect(addr));

		PT_WAIT_THREAD(&ptDS18b20 ,ow->OWWrite(ConvertT_COMMAND));

		//getLink().readDataBit() == false
		PT_WAIT_WHILE(&ptDS18b20 ,ow->OWReadPin()==0);

		PT_WAIT_THREAD(&ptDS18b20 ,ow->OWReset(reset));

		if (reset == EOWResetPresent)
		{
			//match rom
			PT_WAIT_THREAD(&ptDS18b20 ,ow->OWSelect(addr));

			PT_WAIT_THREAD(&ptDS18b20 ,ow->OWWrite(ReadScratchpad_COMMAND));

			PT_WAIT_THREAD(&ptDS18b20,ow->OWRead_bytes((uint8_t*)bytes,sizeof(scratchpadDS1820_t)));

			scratchpadDS1820_t *scratchpad = (scratchpadDS1820_t*)bytes;

			if ( ow->OWCrc8((uint8_t*)bytes,sizeof(scratchpadDS1820_t)) != 0x00 ||
					scratchpad->reserved0!=0xFF)
			{
				error = 1;
			}
			else
			{
				int16_t intPart = (scratchpad->tempMSB<<8) | scratchpad->tempLSB;//(scratchpad.tempMSB==0x00?0x00:0x80) | (scratchpad.tempLSB>>1);

				SetTemp(GetDouble(intPart));

				error = 0;
			}
		}
	}
	else {
		error = 1;
	}

	SetError(error);

	PT_END(&ptDS18b20);
}
