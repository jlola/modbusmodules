#ifndef _DEVICEFUNCTIONS_H_
#define _DEVICEFUNCTIONS_H_

#include "IModbusObjectFactory.h"
#include "IModbusObject.h"
#include "IModbusSlave.h"


#define MAX_OBJECTS  6

class DeviceFunctions
{
	IModbusObject** modbusobjects;
	IModbusSlave* slave;
	IModbusObjectFactory* factory;

public:

	DeviceFunctions(IModbusSlave * pslave, IModbusObjectFactory* factory);

	void Init();

	void Process();
};

#endif
