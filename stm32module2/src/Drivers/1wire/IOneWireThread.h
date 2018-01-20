/*
 * IOneWireThread.h
 *
 *  Created on: 1. 12. 2017
 *      Author: Libor
 */

#ifndef DRIVERS_1WIRE_IONEWIRETHREAD_H_
#define DRIVERS_1WIRE_IONEWIRETHREAD_H_

typedef enum
{
	EOWResetNone = 0,
	EOWResetPresent = 1,
	EOWResetShort = 2
} EOWReset;

class IOneWireThread
{
public:
	virtual uint8_t OWReset(EOWReset & reset)=0;
	virtual uint8_t OWSearch(uint8_t *newAddr,uint8_t & finded_count)=0;
	virtual uint8_t OWCrc8( uint8_t *addr, uint8_t len)=0;
	virtual uint8_t OWSelect(uint8_t rom[8])=0;
	virtual uint8_t OWWrite(uint8_t v)=0;
	virtual uint8_t OWRead_bytes(uint8_t *buf, uint16_t count);
	virtual uint8_t OWReadPin()=0;
};


#endif /* DRIVERS_1WIRE_IONEWIRETHREAD_H_ */
