#ifndef _MODBUSSETTINGS_H_
#define _MODBUSSETTINGS_H_

typedef enum
{
	EDeviceTypeNone,
	EDeviceTypeBinInputs,
	EDeviceTypeBinOutputs,
	EDeviceTypeAInputs,
	EDeviceTypeAOutputs,
	EDeviceTypeDS18B20,
	EDeviceTypeRFID
} EDeviceType;

typedef enum
{
	EModbusReadCoil=0x01,
	EModbusReadDiscreteInput=0x02,
	EModbusReadHoldingRegisters=0x03,
	EModbusReadInputRegisters=0x04,
	EModbusWriteCoil=0x05
} EModbusFunctions;

//------------Modbus---------------------//
#define DEVICEBASE 						0
#define DEVADDR_OFFSET					DEVICEBASE+0
#define COUNT_OF_TYPES_OFFSET			DEVICEBASE+1
#define TYPE_DEFS_OFFSET				DEVICEBASE+2
#define RESET_REG_OFFSET				DEVICEBASE+3
#define LAST_INDEX						DEVICEBASE+4

#define TYPE_DEFS_START_ADDRESS			10
////----------------Types def--------------------//

//
////#include "stm32_lib/inc/stm32f0xx_"
//#include "stm32f0xx.h"
//#include "stm32f0xx_GPIO.h"
//
//#define BUFFER_SIZE		500
//
//#define INVALID_ADDRESS 0xFFFF
//
//#define SLAVERTU_BITINPUTS 100
//#define SLAVERTU_SHORTINPUTS 100
//#define SLAVERTU_COILS 100
//#define SLAVERTU_HOLDINGS 128
//
////OW port defs
//#define OW_PORT			GPIOA
//#define OW_PIN			GPIO_Pin_10
//#define OW_PIN_SOURCE 	GPIO_PinSource10
//#define OW_PIN_BOARD	1
//
////INPUT PORTS
//#define BIN_INPUT1_PORT		GPIOA
//#define BIN_INPUT1_PIN		GPIO_Pin_6
//
//#define BIN_INPUT2_PORT		GPIOA
//#define BIN_INPUT2_PIN		GPIO_Pin_7
//
//
//#define BIN_OUTPUT1_PORT		GPIOA
//#define BIN_OUTPUT1_PIN		GPIO_Pin_9
//
//
//#define BIN_OUTPUT2_PORT		GPIOB
//#define BIN_OUTPUT2_PIN		GPIO_Pin_1
//
//#define BIN_INPUT1_BOARD_PIN	9
//#define BIN_INPUT2_BOARD_PIN	7
//#define BIN_OUTPUT1_BOARD_PIN	5
//#define BIN_OUTPUT2_BOARD_PIN	3
//
//
//typedef enum
//{
//	EDeviceTypeNone,
//	EDeviceTypeBinInputs,
//	EDeviceTypeBinOutputs,
//	EDeviceTypeAInputs,
//	EDeviceTypeAOutputs,
//	EDeviceTypeDS18B20
//} EDeviceType;
//
//typedef enum
//{
//	EModbusReadCoil=0x01,
//	EModbusReadDiscreteInput=0x02,
//	EModbusReadHoldingRegisters=0x03,
//	EModbusReadInputRegisters=0x04,
//	EModbusWriteCoil=0x05
//} EModbusFunctions;
//
////------------Device---------------------//
//#define DEVICEBASE 						0
//#define DEVADDR_OFFSET					DEVICEBASE+0
//#define COUNT_OF_TYPES_OFFSET			DEVICEBASE+1
//#define TYPE_DEFS_OFFSET				DEVICEBASE+2
//#define RESET_REG_OFFSET				DEVICEBASE+3
//#define LAST_INDEX						DEVICEBASE+4
//
//#define COUNT_OF_TYPES_VALUE			3
//#define TYPE_DEFS_VALUE					10
////----------------Types def--------------------//
//#define INPUTS_BIN_TYPE					TYPE_DEFS_VALUE+0
//#define INPUTS_BIN_COUNT				TYPE_DEFS_VALUE+1
//#define INPUTS_BIN_ADDRESS				TYPE_DEFS_VALUE+2
//#define INPUTS_BIN_FUNC					TYPE_DEFS_VALUE+3
//
//#define INPUTS_BIN_FUNC_VALUE			EModbusReadHoldingRegisters
//#define INPUTS_BIN_TYPE_VALUE			EDeviceTypeBinInputs
//#define INPUTS_BIN_COUNT_VALUE			2
//#define INPUTS_BIN_ADDRESS_VALUE		25
//
//#define OUTPUTS_BIN_TYPE				TYPE_DEFS_VALUE+4
//#define OUTPUTS_BIN_COUNT				TYPE_DEFS_VALUE+5
//#define OUTPUTS_BIN_ADDRESS				TYPE_DEFS_VALUE+6
//#define OUTPUTS_BIN_FUNC				TYPE_DEFS_VALUE+7
//
//#define OUTPUTS_BIN_FUNC_VALUE			EModbusReadHoldingRegisters
//#define OUTPUTS_BIN_TYPE_VALUE			EDeviceTypeBinOutputs
//#define OUTPUTS_BIN_COUNT_VALUE			2
//#define OUTPUTS_BIN_ADDRESS_VALUE		30
//
//#define OW_TYPE					TYPE_DEFS_VALUE+8
//#define OW_COUNT				TYPE_DEFS_VALUE+9
//#define OW_ADDRESS				TYPE_DEFS_VALUE+10
//#define OW_FUNC					TYPE_DEFS_VALUE+11
//
//#define OW_FUNC_VALUE			EModbusReadHoldingRegisters
//#define OW_TYPE_VALUE			EDeviceTypeDS18B20
//#define OW_ADDRESS_VALUE		35
//#define OW_COUNT_VALUE			1
//
//
////------------OWManager---------------------//
//#define DS18B20MAXCOUNT 			10
//#define OW_SCAN_OFFSET				OW_ADDRESS_VALUE+0
//#define OW_COUNT_OFFSET 			OW_ADDRESS_VALUE+1
//#define OW_BOARD_PIN				OW_ADDRESS_VALUE+2
//#define OW_RESET_RESULT				OW_ADDRESS_VALUE+3
//
//#define OW_DEVICES_OFFSET 			OW_ADDRESS_VALUE+5
//#define OW_BOARD_PIN_VALUE			1
//
//
#endif
