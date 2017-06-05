#ifndef _IMODBUSREGISTER_H_
#define _IMODBUSREGISTER_H_

#include "stm32f0xx_gpio.h"

//typedef enum {
//	 ModbusErrorOK = 0x00,
// //Function code received in the query is not recognized or allowed by slave
//	 ModbusErrorIllegalFunction = 0x01,
// //Data address of some or all the required entities are not allowed or do not exist in slave
//	 ModbusErrorIllegalAddress = 0x02,
// //Value is not accepted by slave
//	 ModbusErrorIllegalDataValue = 0x03,
// //Unrecoverable error occurred while slave was attempting to perform requested action
//	 ModbusErrorSlaveDeviceFailure = 0x04,
// //Slave has accepted request and is processing it, but a long duration of time is required. This response is returned to prevent a timeout error from occurring in the master.
// //Master can next issue a Poll Program Complete message to determine if processing is completed
//	 ModbusErrorAcknowledge  = 0x05,
// //Slave cannot perform the programming functions. Master should request diagnostic or error information from slave
//	 ModbusErrorNegativeAcknowledge = 0x07
// } ModbusError_t;

class IModbusRegister
{
	virtual void setBitInput(uint16_t index, BitAction state)=0;
	virtual BitAction getBitInput(uint16_t count)=0;

	virtual void setShortInput(uint16_t index, uint16_t val)=0;
	virtual uint16_t getShortInput(uint16_t count)=0;

	virtual void setCoil(uint16_t index, BitAction state)=0;
	virtual BitAction getCoil(uint16_t index)=0;

	virtual void setHolding(uint16_t index, uint16_t val)=0;
	virtual uint16_t getHolding( uint16_t index)=0;
};

#endif
