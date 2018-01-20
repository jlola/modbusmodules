#ifndef _ONEWIRETHREAD_H_
#define _ONEWIRETHREAD_H_

#include "ITimer.h"
#include "ITImerHandler.h"
#include "IOneWireThread.h"
#include "pt.h"
#include "pt-sem.h"

#define BOOL uint8_t

#define FALSE 0
#define TRUE  1

// you can exclude onewire_search by defining that to 0
#ifndef ONEWIRE_SEARCH
#define ONEWIRE_SEARCH 1
#endif

// You can exclude CRC checks altogether by defining this to 0
#ifndef ONEWIRE_CRC
#define ONEWIRE_CRC 1
#endif

// Select the table-lookup method of computing the 8-bit CRC
// by setting this to 1.  The lookup table enlarges code size by
// about 250 bytes.  It does NOT consume RAM (but did in very
// old versions of OneWire).  If you disable this, a slower
// but very compact algorithm is used.
#ifndef ONEWIRE_CRC8_TABLE
#define ONEWIRE_CRC8_TABLE 1
#endif

// You can allow 16-bit CRC checks by defining this to 1
// (Note that ONEWIRE_CRC must also be 1.)
#ifndef ONEWIRE_CRC16
#define ONEWIRE_CRC16 1
#endif

typedef struct{
  GPIO_TypeDef*         m_Port;
  uint32_t              Pin;
  uint32_t 				PinSource;
#ifdef ONEWIRE_SEARCH
  // global search state
  unsigned char ROM_NO[8];
  uint8_t LastDiscrepancy;
  uint8_t LastFamilyDiscrepancy;
  uint8_t LastDeviceFlag;
#endif
}OWire;


class OneWireThread : public IOneWireThread, public ITimerHandler
{
	typedef enum {
		ERunFuncNone,
		ERunFuncReset,
		ERunFuncWriteBit,
		ERunFuncReadBit,
	} ERunFunc;

	ERunFunc runFunc;

	ITimer* timer;
	pt ptDS18b20;
	pt ptWriteBit;
	pt ptReadBit;
	pt ptWriteBitTimer;
	pt ptReadBitTimer;
	pt ptResetTimer;
	pt ptReset;
	pt ptOWSearch;
	pt ptOWWrite;
	pt ptOWWriteBytes;
	pt ptOWRead;
	pt ptOWReadBytes;
	pt ptOWSelect;
	pt ptOWRead_bytes;

	pt_sem ptWriteBitTimerSem;
	pt_sem ptReadBitTimerSem;
	pt_sem ptResetTimerSem;
	uint32_t delayus;
	uint8_t bit;
	uint8_t pinValue;
	EOWReset resetTimerResult;

	uint8_t OWWrite_bit_Timer(uint8_t v);
	uint8_t OWReset_Timer();

	void OnHWTimer(uint8_t us);
public:

	OWire owirestruct;
	OWire* owire;
	OneWireThread(ITimer* ptimer,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t GPIO_PinSource);
	inline void DelayuS(uint32_t nCount);

	void OWInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t GPIO_PinSource);

	void OWInput();
	void OWOutput();
	uint8_t OWReadPin();
	inline void OWWriteHigh();
	inline void OWWriteLow();


	void OWInterrupts(void);
	void OWNoInterrupts(void);

	uint8_t OWReset(EOWReset & reset);

	#ifdef ONEWIRE_SEARCH
	void OWReset_search(OWire* owire);
	uint8_t OWSearch(uint8_t *newAddr,uint8_t & result);
	#endif

	uint8_t OWRead_bit_Timer(uint8_t & bit);

	uint8_t OWRead_bit(uint8_t & bit);

	uint8_t OWWrite_bit(uint8_t v);

	uint8_t OWWrite(uint8_t v);

	uint8_t OWWrite_bytes(const uint8_t *buf, uint16_t count);

	uint8_t OWRead(OWire* owire, uint8_t * byte);

	uint8_t OWRead_bytes(uint8_t *buf, uint16_t count);

	uint8_t  OWSelect(uint8_t rom[8]);

	void OWSkip(OWire* owire);

	void OWDepower(OWire* owire);

	#ifdef ONEWIRE_CRC
		uint8_t OWCrc8( uint8_t *addr, uint8_t len);
	#ifdef ONEWIRE_CRC16
		uint8_t OWCheck_crc16(uint8_t* input, uint16_t len, uint8_t* inverted_crc);
		uint16_t OWCrc16(uint8_t* input, uint16_t len);
	#endif

	#endif
};



#endif
