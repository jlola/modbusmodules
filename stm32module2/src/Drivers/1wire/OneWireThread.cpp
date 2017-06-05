#include <string.h>
#include "OneWireThread.h"


OneWireThread::OneWireThread(ITimer* ptimer,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin,uint16_t GPIO_PinSource)
	: runFunc(ERunFuncNone),timer(ptimer)
{
	timer->SetReceiver(this);
	PT_INIT(&ptWriteBit);
	PT_INIT(&ptReadBit);
	PT_INIT(&ptWriteBitTimer);
	PT_INIT(&ptReadBitTimer);
	PT_INIT(&ptResetTimer);
	PT_INIT(&ptReset);

	PT_INIT(&ptDS18b20);
	PT_INIT(&ptOWSearch);
	PT_INIT(&ptOWWrite);
	PT_INIT(&ptOWWriteBytes);
	PT_INIT(&ptOWRead);
	PT_INIT(&ptOWReadBytes);
	PT_INIT(&ptOWSelect);
	PT_INIT(&ptOWRead_bytes);

	PT_SEM_INIT(&ptWriteBitTimerSem,0);
	PT_SEM_INIT(&ptReadBitTimerSem,0);
	PT_SEM_INIT(&ptResetTimerSem,0);

	OWInit(GPIOx,GPIO_Pin,GPIO_PinSource);
}

void OneWireThread::OnHWTimer(uint8_t us)
{
	if (delayus > us)
		delayus-=us;
	else
		delayus = 0;

	if (delayus==0)
	{
		switch(runFunc)
		{
			case ERunFuncReadBit:
				OWRead_bit_Timer(bit);
				break;
			case ERunFuncReset:
				OWReset_Timer();
				break;
			case ERunFuncWriteBit:
				OWWrite_bit_Timer(bit);
				break;
			case ERunFuncNone:
				break;
		}

		//if (PT_SCHEDULE(ptReadBitTimer.lc))
		//	OWWrite_bit_Timer(owire,bit);
	}
}

inline void OneWireThread::DelayuS(uint32_t nCount)
{
	if ( nCount < 10)
		delayus = 1;
	else
		delayus = nCount;
}

void OneWireThread::OWInit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint16_t GPIO_PinSource)
{
	GPIO_InitTypeDef gpioinit;
	//GPIO_StructInit(gpioinit);
	owire = &owirestruct;
	gpioinit.GPIO_Mode = GPIO_Mode_IN;
	gpioinit.GPIO_Pin = GPIO_Pin;
	gpioinit.GPIO_PuPd = GPIO_PuPd_UP;
	gpioinit.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&gpioinit);
	owire->m_Port = GPIOx;
	owire->Pin = GPIO_Pin;
	owire->PinSource = GPIO_PinSource;
//#ifdef ONEWIRE_SEARCH
	//OWReset_search(owire);
//#endif
}

//set direction
void OneWireThread::OWInput()
{
//	GPIO_InitTypeDef gpioinit;
//	//GPIO_StructInit(gpioinit);
//	gpioinit.GPIO_Mode = GPIO_Mode_IN;
//	gpioinit.GPIO_Pin = owire->Pin;
//	gpioinit.GPIO_PuPd = GPIO_PuPd_UP;
//	gpioinit.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(owire->m_Port,&gpioinit);

	owire->m_Port->MODER  &= ~(GPIO_MODER_MODER0 << (owire->PinSource * 2));
	owire->m_Port->MODER |= (((uint32_t)GPIO_Mode_IN) << (owire->PinSource * 2));
}
//set direction
void OneWireThread::OWOutput()
{
//	GPIO_InitTypeDef gpioinit;
//	//GPIO_StructInit(gpioinit);
//	gpioinit.GPIO_Mode = GPIO_Mode_OUT;
//	gpioinit.GPIO_Pin = owire->Pin;
//	gpioinit.GPIO_OType = GPIO_OType_OD;
//	gpioinit.GPIO_PuPd = GPIO_PuPd_UP;
//	gpioinit.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(owire->m_Port,&gpioinit);

	owire->m_Port->MODER  &= ~(GPIO_MODER_MODER0 << (owire->PinSource * 2));
	owire->m_Port->MODER |= (((uint32_t)GPIO_Mode_OUT) << (owire->PinSource * 2));
}

uint8_t OneWireThread::OWReadPin()
{
	//if (GPIO_ReadInputDataBit(owire->m_Port,owire->Pin)==SET)
	//	return 1;
	//return 0;
	if ((owire->m_Port->IDR & owire->Pin) != (uint32_t)Bit_RESET)
	  {
	    return 1;
	  }
	  else
	  {
	    return 0;
	  }
}

inline void OneWireThread::OWWriteHigh()
{
	//GPIO_WriteBit(owire->m_Port,owire->Pin, Bit_SET);
	owire->m_Port->BSRR = owire->Pin ;
}

inline void OneWireThread::OWWriteLow()
{
	//GPIO_WriteBit(owire->m_Port,owire->Pin, Bit_RESET);
	owire->m_Port->BRR = owire->Pin ;
}

void OneWireThread::OWNoInterrupts(void)
{
  //__asm("cpsid i");
}

void OneWireThread::OWInterrupts(void)
{
  //__asm("cpsie i");
}

uint8_t OneWireThread::OWReset(EOWReset & reset)
{
	reset = EOWResetNone;
	runFunc = ERunFuncReset;
	PT_BEGIN(&ptReset);
	PT_SEM_INIT(&ptResetTimerSem,0);
	//OWOutput(owire);
	DelayuS(0);
	timer->Start(this);
	PT_SEM_WAIT(&ptReset,&ptResetTimerSem);
	reset = resetTimerResult;
	PT_END(&ptReset);
}

uint8_t OneWireThread::OWReset_Timer()
{
	static uint8_t retries;

	PT_BEGIN(&ptResetTimer);

	retries = 125;

	OWInput();
	// wait until the wire is high... just in case
	if (!OWReadPin())
	{
		do {
			if (--retries == 0)
			{
				PT_SEM_SIGNAL(&ptResetTimer,&ptResetTimerSem);
				resetTimerResult = EOWResetShort;
				timer->Stop();
				PT_EXIT(&ptResetTimer);
			}
			DelayuS(2);//DelayuS(2);
			PT_YIELD(&ptResetTimer);
		} while ( !OWReadPin());
	}

	OWWriteLow();
	OWOutput();	// drive output low
	DelayuS(500);
	PT_YIELD(&ptResetTimer);
	OWWriteHigh();
	OWInput();	// allow it to float
	DelayuS(70);
	PT_YIELD(&ptResetTimer);
	if (!OWReadPin())
		resetTimerResult = EOWResetPresent;
	else
		resetTimerResult = EOWResetNone;
	DelayuS(410);
	PT_YIELD(&ptResetTimer);
	PT_SEM_SIGNAL(&ptResetTimer,&ptResetTimerSem);
	timer->Stop();
	PT_END(&ptResetTimer);
}

uint8_t OneWireThread::OWWrite_bit_Timer(uint8_t v)
{
	PT_BEGIN(&ptWriteBitTimer);
	if (v & 1) {
		OWWriteLow();
		OWOutput();
		//OWOutput(owire);	// drive output low
		DelayuS(5);
		PT_YIELD(&ptWriteBitTimer);
		OWWriteHigh();	// drive output high
		DelayuS(55);
		PT_YIELD(&ptWriteBitTimer);
	} else {
		OWWriteLow();
		OWOutput();
		//OWOutput(owire);	// drive output low
		DelayuS(65);
		PT_YIELD(&ptWriteBitTimer);
		OWWriteHigh();	// drive output high
		DelayuS(5);
		PT_YIELD(&ptWriteBitTimer);
	}
	PT_SEM_SIGNAL(&ptWriteBitTimer,&ptWriteBitTimerSem);
	timer->Stop();
	PT_END(&ptWriteBitTimer);
}

uint8_t OneWireThread::OWWrite_bit(uint8_t v)
{
	bit = v;
	runFunc = ERunFuncWriteBit;
	PT_BEGIN(&ptWriteBit);
	PT_SEM_INIT(&ptWriteBitTimerSem,0);
	//OWOutput();
	//OWWrite_bit_Timer(owire,bit);
	DelayuS(0);
	timer->Start(this);
	PT_SEM_WAIT(&ptWriteBit,&ptWriteBitTimerSem);

	PT_END(&ptWriteBit);
}

uint8_t OneWireThread::OWRead_bit(uint8_t & bit)
{
	runFunc = ERunFuncReadBit;
	PT_BEGIN(&ptReadBit);
	PT_SEM_INIT(&ptReadBitTimerSem,0);
	DelayuS(0);
	timer->Start(this);
	PT_SEM_WAIT(&ptReadBit,&ptReadBitTimerSem);
	bit = this->bit;
	PT_END(&ptReadBit);
}

uint8_t OneWireThread::OWRead_bit_Timer(uint8_t & mbit)
{
	uint8_t r = 0;
	PT_BEGIN(&ptReadBitTimer);
    OWWriteLow();
	OWOutput();
	DelayuS(5);
	PT_YIELD(&ptReadBitTimer);
	OWInput();	// let pin float, pull up will raise
	DelayuS(2);
	PT_YIELD(&ptReadBitTimer);
	r = OWReadPin();
	mbit = r;
	DelayuS(55);
	PT_YIELD(&ptReadBitTimer);
	PT_SEM_SIGNAL(&ptReadBitTimer,&ptReadBitTimerSem);
	timer->Stop();
	PT_END(&ptReadBitTimer);
}

uint8_t OneWireThread::OWWrite(uint8_t v) {

	static uint8_t bitMaskWrite;
    PT_BEGIN(&ptOWWrite);
    for (bitMaskWrite = 0x01; bitMaskWrite; bitMaskWrite <<= 1) {
    	 PT_WAIT_THREAD(&ptOWWrite,OWWrite_bit((bitMaskWrite & v)?1:0));
    }

    OWInput();
    OWWriteLow();

    PT_END(&ptOWWrite);
}

uint8_t OneWireThread::OWWrite_bytes(const uint8_t *buf, uint16_t count) {
  static uint16_t iwritebytes;

  PT_BEGIN(&ptOWReadBytes);

  for (iwritebytes = 0 ; iwritebytes < count ; iwritebytes++)
    PT_WAIT_THREAD(&ptOWReadBytes,OWWrite(buf[iwritebytes]));

  PT_END(&ptOWReadBytes);
}

uint8_t OneWireThread::OWRead(OWire* owire, uint8_t * byte) {
    static uint8_t bitMaskRead;
    static uint8_t rOwRead = 0;
    static uint8_t bitOwRead = 0;

    PT_BEGIN(&ptOWRead);
    rOwRead = 0;
    for (bitMaskRead = 0x01; bitMaskRead; bitMaskRead <<= 1) {
    	PT_WAIT_THREAD(&ptOWRead,OWRead_bit(bitOwRead));
    	if ( bitOwRead ) rOwRead |= bitMaskRead;
    }
    *byte = rOwRead;
    PT_END(&ptOWRead);

}

uint8_t OneWireThread::OWRead_bytes(uint8_t *buf, uint16_t count) {
  static uint16_t ireadbytes;
  uint8_t b=0;
  PT_BEGIN(&ptOWRead_bytes);
  for (ireadbytes = 0 ; ireadbytes < count ; ireadbytes++)
  {
	  PT_WAIT_THREAD(&ptOWRead_bytes, OWRead(owire,&b));
	  buf[ireadbytes] = b;
  }
  PT_END(&ptOWRead_bytes);
}

uint8_t OneWireThread::OWSelect(uint8_t rom[8])
{
    static int iwselect;
    PT_BEGIN(&ptOWSelect);

    PT_WAIT_THREAD(&ptOWSelect,OWWrite(0x55));           // Choose ROM

    for( iwselect = 0; iwselect < 8; iwselect++)
    {
    	PT_WAIT_THREAD(&ptOWSelect,OWWrite(rom[iwselect]));
    }

    PT_END(&ptOWSelect);
}

void OneWireThread::OWSkip(OWire* owire)
{
	OWWrite(0xCC);           // Skip ROM
}

void OneWireThread::OWDepower(OWire* owire)
{
	OWNoInterrupts();
	OWInput();
	OWInterrupts();
}

#ifdef ONEWIRE_SEARCH

void OneWireThread::OWReset_search(OWire* owire)
{
  static int i;
  // reset the search state
  owire->LastDiscrepancy = 0;
  owire->LastDeviceFlag = FALSE;
  owire->LastFamilyDiscrepancy = 0;
  for(i = 7; ; i--)
  {
    owire->ROM_NO[i] = 0;
    if ( i == 0) break;
  }
}

uint8_t OneWireThread::OWSearch(uint8_t *newAddr,uint8_t & finded_count)
{
   static uint8_t id_bit_number;
   static uint8_t last_zero, rom_byte_number, search_result;
   static uint8_t id_bit, cmp_id_bit;
   static int i;
   static unsigned char rom_byte_mask, search_direction;

   PT_BEGIN(&ptOWSearch);
   // initialize for search
   memset(newAddr,0x00,8);
   id_bit_number = 1;
   last_zero = 0;
   rom_byte_number = 0;
   rom_byte_mask = 1;
   search_result = 0;

   // if the last call was not the last one
   if (!owire->LastDeviceFlag)
   {
	   EOWReset reset;
	   PT_WAIT_THREAD(&ptOWSearch,OWReset(reset));
      // 1-Wire reset
      if (reset != EOWResetPresent)
      {
         // reset the search
         owire->LastDiscrepancy = 0;
         owire->LastDeviceFlag = FALSE;
         owire->LastFamilyDiscrepancy = 0;
         finded_count = 0;
         PT_EXIT(&ptOWSearch);
         //return FALSE;
      }

      // issue the search command
      PT_WAIT_THREAD(&ptOWSearch,OWWrite(0xF0));

      // loop to do the search
      do
      {
         // read a bit and its complement
    	  PT_WAIT_THREAD(&ptOWSearch,OWRead_bit(id_bit));

    	  PT_WAIT_THREAD(&ptOWSearch,OWRead_bit(cmp_id_bit));

         // check for no devices on 1-wire
         if ((id_bit == 1) && (cmp_id_bit == 1))
            break;
         else
         {
            // all devices coupled have 0 or 1
            if (id_bit != cmp_id_bit)
               search_direction = id_bit;  // bit write value for search
            else
            {
               // if this discrepancy if before the Last Discrepancy
               // on a previous next then pick the same as last time
               if (id_bit_number < owire->LastDiscrepancy)
                  search_direction = ((owire->ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
               else
                  // if equal to last pick 1, if not then pick 0
                  search_direction = (id_bit_number == owire->LastDiscrepancy);

               // if 0 was picked then record its position in LastZero
               if (search_direction == 0)
               {
                  last_zero = id_bit_number;

                  // check for Last discrepancy in family
                  if (last_zero < 9)
                     owire->LastFamilyDiscrepancy = last_zero;
               }
            }

            // set or clear the bit in the ROM byte rom_byte_number
            // with mask rom_byte_mask
            if (search_direction == 1)
              owire->ROM_NO[rom_byte_number] |= rom_byte_mask;
            else
              owire->ROM_NO[rom_byte_number] &= ~rom_byte_mask;

            // serial number search direction write bit
            PT_WAIT_THREAD(&ptOWSearch,OWWrite_bit(search_direction));

            // increment the byte counter id_bit_number
            // and shift the mask rom_byte_mask
            id_bit_number++;
            rom_byte_mask <<= 1;

            // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
            if (rom_byte_mask == 0)
            {
                rom_byte_number++;
                rom_byte_mask = 1;
            }
         }
      }
      while(rom_byte_number < 8);  // loop until through all ROM bytes 0-7

      // if the search was successful then
      if (!(id_bit_number < 65))
      {
         // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
         owire->LastDiscrepancy = last_zero;

         // check for last device
         if (owire->LastDiscrepancy == 0)
            owire->LastDeviceFlag = TRUE;

         search_result = TRUE;
      }


   }

   // if no device found then reset counters so next 'search' will be like a first
   if (!search_result || !owire->ROM_NO[0])
   {
      owire->LastDiscrepancy = 0;
      owire->LastDeviceFlag = FALSE;
      owire->LastFamilyDiscrepancy = 0;
      search_result = FALSE;
   }
   for (i = 0; i < 8; i++) newAddr[i] = owire->ROM_NO[i];
   //return search_result;
   finded_count = search_result;
   PT_END(&ptOWSearch);
  }
#endif

#ifdef ONEWIRE_CRC
// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
//

#ifdef ONEWIRE_CRC8_TABLE
// This table comes from Dallas sample code where it is freely reusable,
// though Copyright (C) 2000 Dallas Semiconductor Corporation
static const uint8_t dscrc_table[] = {
      0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
    157,195, 33,127,252,162, 64, 30, 95,  1,227,189, 62, 96,130,220,
     35,125,159,193, 66, 28,254,160,225,191, 93,  3,128,222, 60, 98,
    190,224,  2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
     70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89,  7,
    219,133,103, 57,186,228,  6, 88, 25, 71,165,251,120, 38,196,154,
    101, 59,217,135,  4, 90,184,230,167,249, 27, 69,198,152,122, 36,
    248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91,  5,231,185,
    140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
     17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
    175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
     50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
    202,148,118, 40,171,245, 23, 73,  8, 86,180,234,105, 55,213,139,
     87,  9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
    233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
    116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

//
// Compute a Dallas Semiconductor 8 bit CRC. These show up in the ROM
// and the registers.  (note: this might better be done without to
// table, it would probably be smaller and certainly fast enough
// compared to all those delayMicrosecond() calls.  But I got
// confused, so I use this table from the examples.)
//
uint8_t OneWireThread::OWCrc8( uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		crc = dscrc_table[(crc ^ *addr++)];
	}
	return crc;
}
#else
//
// Compute a Dallas Semiconductor 8 bit CRC directly.
// this is much slower, but much smaller, than the lookup table.
//
uint8_t OneWireThread::OWCrc8( uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;

	while (len--) {
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--) {
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
			inbyte >>= 1;
		}
	}
	return crc;
}
#endif

#ifdef ONEWIRE_CRC16
uint8_t OneWireThread::OWCheck_crc16(uint8_t* input, uint16_t len, uint8_t* inverted_crc)
{
    uint16_t crc = ~OWCrc16(input, len);
    return (crc & 0xFF) == inverted_crc[0] && (crc >> 8) == inverted_crc[1];
}

uint16_t OneWireThread::OWCrc16(uint8_t* input, uint16_t len)
{
    static const uint8_t oddparity[16] = { 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0 };
    uint16_t crc = 0;    // Starting seed is zero.
    uint16_t i;
    for (i = 0 ; i < len ; i++) {
      // Even though we're just copying a byte from the input,
      // we'll be doing 16-bit computation with it.
      uint16_t cdata = input[i];
      cdata = (cdata ^ (crc & 0xff)) & 0xff;
      crc >>= 8;

      if (oddparity[cdata & 0x0F] ^ oddparity[cdata >> 4])
          crc ^= 0xC001;

      cdata <<= 6;
      crc ^= cdata;
      cdata <<= 1;
      crc ^= cdata;
    }
    return crc;
}
#endif

#endif
