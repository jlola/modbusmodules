/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------
#ifndef UNITTEST

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "IOPin.h"
#include "Settings.h"
#include "Queue.h"
#include "Random.h"
#include "IOPin.h"
#include "RS485.h"
#include "DeviceFunctions.h"
#include "OneWireManager.h"
#include "OneWireThread.h"
#include "Timer6.h"
#include "Timer3.h"
#include "slave-rtu.h"
#include "ModbusSettings.h"
#include "string.h"
#include <stm32f0xx_tim.h>
#include <USART1.h>
#include "USART2.h"
#include "CFlash.h"
#include "IWDG/Watchdog.h"
#include "diag/Trace.h"
#include "Timer14.h"
#include "ModbusObjectFactory.h"

// ----------------------------------------------------------------------------
//
// Standalone STM32F0 empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//#pragma GCC diagnostic ignored "-Wmissing-declarations"
//#pragma GCC diagnostic ignored "-Wreturn-type"

//unsigned int __attribute__((section (".b1text"))) addr __attribute__ ((aligned (4))) = 0x00000002;
unsigned short addr __attribute__ ((section (".b1text"))) = 0x02;
int i;

int
main(int argc, char* argv[])
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOF, ENABLE);
	trace_initialize();

	//for compilation intialization of memory addr
	i = addr;

	trace_printf("System clock: %u Hz\n", SystemCoreClock);

	IOPin powerled(POWER_LED_PORT,POWER_LED_PIN,IODirection::IOOutput,0, true);

#ifdef STM32F030C8
	USARTBase* usart2 = CUSART2::Instance();
	usart2->Init(9600);
#endif
	USARTBase* usart1 = CUSART1::Instance();
	usart1->Init(RS485_SPEED);

	Timer3::Instance()->Init();
	OneWireThread owthread(Timer3::Instance(),OW_PORT,OW_PIN,OW_PIN_SOURCE);
	Timer14::Instance()->Init();


//	//pin for receive enabled driving
	IOPin recEnable(REC_ENABLE_PORT,REC_ENABLE_PIN,IOOutput,0, true);

	RS485 rs485(usart1, NULL,&recEnable, 4000000/RS485_SPEED, Timer14::Instance());

	uint16_t old[2];
	CFlash::Read16(old,1);
	SlaveRtu slave(rs485,old[0]);
	OneWireManager owmanager(&owthread,&slave);

#ifdef STM32F030C8
	Timer6::Instance()->Init();
	ModbusObjectFactory factory(&slave,usart2,Timer6::Instance(),&owmanager);
#endif
#ifdef STM32F030K6
	ModbusObjectFactory factory(&slave,&owmanager);
#endif

	DeviceFunctions dev(&slave, &factory);

	owmanager.StartScan();

	rs485.RecEnable(true);
	recEnable.Set(false);
	trace_puts("Started");
	while(1)
	{
		//usart2->Send(sentstr,0,strlen(sentstr));
		dev.Process();
		slave.ReceiveData();
		//int rnd = getTrueRandomNumber();
	}
}

#pragma GCC diagnostic pop

#endif

// ----------------------------------------------------------------------------
