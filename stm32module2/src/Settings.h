#ifndef _SETTINGS_H_
#define _SETTINGS_H_

//#define SMALL_SWITCH
//#define SMALL_SWITCH_1INPUT
#define BIGRELIO
//#define RFID

/***************************************************************/

#if defined(SMALL_SWITCH) || defined(SMALL_SWITCH_1INPUT)

#ifdef STM32F030C8
#error "This cpu should not be defined"
#endif

#ifndef STM32F030K6
#define STM32F030K6
#endif

#ifndef STM32F030
#define STM32F030
#endif

#define POWER_LED_PIN			GPIO_Pin_0
#define POWER_LED_PORT			GPIOB

#define REC_ENABLE_PORT	GPIOA
#define REC_ENABLE_PIN  GPIO_Pin_0

//OW port defs
#define OW_PORT			GPIOA
#define OW_PIN			GPIO_Pin_15
#define OW_PIN_SOURCE 	GPIO_PinSource15
#define OW_PIN_BOARD	1

//INPUT PORTS
#ifdef SMALL_SWITCH_1INPUT

#define BIN_INPUT1_PORT		GPIOA
#define BIN_INPUT1_PIN		GPIO_Pin_5

#else

#define BIN_INPUT1_PORT		GPIOA
#define BIN_INPUT1_PIN		GPIO_Pin_5

#define BIN_INPUT2_PORT		GPIOA
#define BIN_INPUT2_PIN		GPIO_Pin_6

#define BIN_INPUT3_PORT		GPIOA
#define BIN_INPUT3_PIN		GPIO_Pin_7

#endif

#define BIN_OUTPUT1_PORT		GPIOB
#define BIN_OUTPUT1_PIN		GPIO_Pin_1

#define BIN_INPUT1_BOARD_PIN	1
#define BIN_INPUT2_BOARD_PIN	2
#define BIN_INPUT3_BOARD_PIN	3
#define BIN_OUTPUT1_BOARD_PIN	4
/******************************************************/
#elif defined BIGRELIO

#ifdef STM32F030C8
#error "This cpu should not be defined"
#endif

#ifndef STM32F030K6
#define STM32F030K6
#endif

#ifndef STM32F030
#define STM32F030
#endif

#define POWER_LED_PIN			GPIO_Pin_0
#define POWER_LED_PORT			GPIOB

#define REC_ENABLE_PORT	GPIOA
#define REC_ENABLE_PIN  GPIO_Pin_0

//OW port defs
#define OW_PORT			GPIOA
#define OW_PIN			GPIO_Pin_15
#define OW_PIN_SOURCE 	GPIO_PinSource15
#define OW_PIN_BOARD	1

#define BIN_INPUT1_PORT		GPIOA
#define BIN_INPUT1_PIN		GPIO_Pin_12

#define BIN_INPUT2_PORT		GPIOB
#define BIN_INPUT2_PIN		GPIO_Pin_3

#define BIN_INPUT3_PORT		GPIOB
#define BIN_INPUT3_PIN		GPIO_Pin_4

#define BIN_INPUT4_PORT		GPIOB
#define BIN_INPUT4_PIN		GPIO_Pin_5

#define BIN_INPUT5_PORT		GPIOB
#define BIN_INPUT5_PIN		GPIO_Pin_6

#define BIN_INPUT6_PORT		GPIOB
#define BIN_INPUT6_PIN		GPIO_Pin_7


#define BIN_OUTPUT7_PORT		GPIOA
#define BIN_OUTPUT7_PIN		GPIO_Pin_10

#define BIN_OUTPUT8_PORT		GPIOA
#define BIN_OUTPUT8_PIN		GPIO_Pin_7

#define BIN_OUTPUT9_PORT		GPIOA
#define BIN_OUTPUT9_PIN		GPIO_Pin_11

#define BIN_OUTPUT10_PORT		GPIOA
#define BIN_OUTPUT10_PIN		GPIO_Pin_9

#define BIN_OUTPUT11_PORT		GPIOA
#define BIN_OUTPUT11_PIN		GPIO_Pin_8

#define BIN_OUTPUT12_PORT		GPIOA
#define BIN_OUTPUT12_PIN		GPIO_Pin_6

#define BIN_OUTPUT13_PORT		GPIOB
#define BIN_OUTPUT13_PIN		GPIO_Pin_1

#define BIN_OUTPUT14_PORT		GPIOA
#define BIN_OUTPUT14_PIN		GPIO_Pin_5

#define BIN_OUTPUT15_PORT		GPIOA
#define BIN_OUTPUT15_PIN		GPIO_Pin_4

#define BIN_INPUT1_BOARD_PIN	1
#define BIN_INPUT2_BOARD_PIN	2
#define BIN_INPUT3_BOARD_PIN	3
#define BIN_INPUT4_BOARD_PIN	4
#define BIN_INPUT5_BOARD_PIN	5
#define BIN_INPUT6_BOARD_PIN	6

#define BIN_OUTPUT7_BOARD_PIN	7
#define BIN_OUTPUT8_BOARD_PIN	8
#define BIN_OUTPUT9_BOARD_PIN	9
#define BIN_OUTPUT10_BOARD_PIN	10
#define BIN_OUTPUT11_BOARD_PIN	11
#define BIN_OUTPUT12_BOARD_PIN	12
#define BIN_OUTPUT13_BOARD_PIN	13
#define BIN_OUTPUT14_BOARD_PIN	14
#define BIN_OUTPUT15_BOARD_PIN	15

/******************************************************/
#elif defined RFID

#ifndef STM32F030C8
#define STM32F030C8
#endif

#ifndef STM32F030xC
#define STM32F030xC
#endif

#define REC_ENABLE_PORT	GPIOA
#define REC_ENABLE_PIN  GPIO_Pin_11

//OW port defs
#define OW_PORT			GPIOA
#define OW_PIN			GPIO_Pin_6
#define OW_PIN_SOURCE 	GPIO_PinSource6
#define OW_PIN_BOARD	1

#define BIN_INPUT1_PORT		GPIOB
#define BIN_INPUT1_PIN		GPIO_Pin_0
#define BIN_INPUT1_BOARD_PIN	2

#define BIN_OUTPUT1_PORT		GPIOB
#define BIN_OUTPUT1_PIN		GPIO_Pin_1
#define BIN_OUTPUT1_BOARD_PIN	3


#define POWER_LED_PIN			GPIO_Pin_6
#define POWER_LED_PORT			GPIOF

#else
#error "Target board not selected"
#endif


#define RS485_SPEED 115200
#define BUFFER_SIZE		256

#define INVALID_ADDRESS 0xFFFF

#define SLAVERTU_BITINPUTS 100
#define SLAVERTU_SHORTINPUTS 100
#define SLAVERTU_COILS 100
#define SLAVERTU_HOLDINGS 200


#endif
