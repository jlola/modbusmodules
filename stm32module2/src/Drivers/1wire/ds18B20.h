#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "OneWireThread.h"
#include "slave-rtu.h"

#define DS18B20_SIZE		6

#define DS18B20_ADDRBYTE01 	0
#define DS18B20_ADDRBYTE23 	1
#define DS18B20_ADDRBYTE45 	2
#define DS18B20_ADDRBYTE67 	3
#define DS18B20_TEMPER		4
#define DS18B20_ERROR		5

class DS18B20
{
	//=====================================================================================
	  //
	  // constants
	  //

	  /** Family code of this specific device. */
	  static const uint8_t  familyCode = 0x28;

	  typedef uint8_t owCommand_t;

	  static const owCommand_t  ConvertT_COMMAND         = 0x44; /**< 1-Wire command byte constant */
	  static const owCommand_t  ReadScratchpad_COMMAND   = 0xbe; /**< 1-Wire command byte constant */
	  static const owCommand_t  WriteScratchpad_COMMAND  = 0x4e; /**< 1-Wire command byte constant */
	  static const owCommand_t  CopyScratchpad_COMMAND   = 0x48; /**< 1-Wire command byte constant */
	  static const owCommand_t  RecallE2_COMMAND         = 0xb8; /**< 1-Wire command byte constant */
	  static const owCommand_t  ReadPowerSupply_COMMAND  = 0xb4; /**< 1-Wire command byte constant */


	  //=====================================================================================
	  //
	  // type definitions
	  //

	  /** DS1820 internal scratchpad as defined by Dallas. */
  typedef struct {
	uint8_t  tempLSB;
	uint8_t  tempMSB;
	uint8_t  tH;
	uint8_t  tL;
	uint8_t  configuration;
	uint8_t  reserved0;
	uint8_t  reserved1;
	uint8_t  reserved2;
	uint8_t  crc8;
  } scratchpadDS1820_t;

//  typedef struct {
//	  uint8_t addr[8];
//	  uint16_t Temp;
//	  uint16_t ErrorCode;
//  } modbusData_t;

  	uint8_t addr[8];
	OneWireThread* ow;
	SlaveRtu* slave;
	uint8_t baseAddr;
	pt ptDS18b20;
public:
	static const uint8_t Size = 4 + 1 + 1;

	DS18B20();
	void Init(OneWireThread* thread,uint8_t * owAddr,SlaveRtu* Slave, uint8_t baseAddr);
	uint8_t Read();
	int16_t GetDouble(uint16_t pValue);

	uint16_t GetError();
	void SetError(uint16_t error);

	uint16_t GetTemp();
	void SetTemp(uint16_t error);
};


#endif
