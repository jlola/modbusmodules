#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define SMALL_SWITCH
//#define RFID

#ifdef SMALL_SWITCH

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


//OW port defs
#define OW_PORT			GPIOA
#define OW_PIN			GPIO_Pin_15
#define OW_PIN_SOURCE 	GPIO_PinSource15
#define OW_PIN_BOARD	1

//INPUT PORTS
#define BIN_INPUT1_PORT		GPIOA
#define BIN_INPUT1_PIN		GPIO_Pin_5

#define BIN_INPUT2_PORT		GPIOA
#define BIN_INPUT2_PIN		GPIO_Pin_6

#define BIN_INPUT3_PORT		GPIOA
#define BIN_INPUT3_PIN		GPIO_Pin_7

#define BIN_OUTPUT1_PORT		GPIOB
#define BIN_OUTPUT1_PIN		GPIO_Pin_1

#define BIN_INPUT1_BOARD_PIN	1
#define BIN_INPUT2_BOARD_PIN	2
#define BIN_INPUT3_BOARD_PIN	3
#define BIN_OUTPUT1_BOARD_PIN	4

#endif

#ifdef RFID

#ifndef STM32F030C8
#define STM32F030C8
#endif

#ifndef STM32F030xC
#define STM32F030xC
#endif

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


#endif





#define RS485_SPEED 115200
#define BUFFER_SIZE		500

#define INVALID_ADDRESS 0xFFFF

#define SLAVERTU_BITINPUTS 100
#define SLAVERTU_SHORTINPUTS 100
#define SLAVERTU_COILS 100
#define SLAVERTU_HOLDINGS 200


#endif
