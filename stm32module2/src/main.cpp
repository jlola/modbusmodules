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
#include "Timer.h"
#include "Timer3.h"
#include "slave-rtu.h"
#include "ModbusSettings.h"
#include "string.h"
#include <stm32f0xx_tim.h>
#include <USART1.h>
#include "CFlash.h"
#include "IWDG/Watchdog.h"

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

char buffer[BUFFER_SIZE];

int
main(int argc, char* argv[])
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOA, ENABLE);

	IOPin powerled(POWER_LED_PORT,POWER_LED_PIN,IODirection::IOOutput,0);
	powerled.Set(true);

	USARTBase* usart1 = CUSART1::Instance();
	Timer3::Instance()->Init();
	usart1->Init(RS485_SPEED);

	OneWireThread owthread(Timer3::Instance(),OW_PORT,OW_PIN,OW_PIN_SOURCE);

//	//pin for receive enabled driving
	IOPin pre(GPIOA,GPIO_Pin_0,IOOutput,0);
	pre.Set(false);
	RS485 rs485(usart1, NULL,&pre, 4*1000000/RS485_SPEED);
	RS485SetInstance(&rs485);

	SlaveRtu slave(rs485,1);
	OneWireManager owmanager(&owthread,&slave);
	DeviceFunctions dev(&slave,&owmanager);
	dev.Init();
	owmanager.StartScan();

	char pdata;
	size_t i = 0;
	rs485.RecEnable(true);
	while(1)
	{
		owmanager.Refresh();
		dev.Process();
		if (rs485.PacketCompleted())
		{
			//send_command(0x05/* some interrupt ID */, m);
			//read packet
			while(rs485.Read(pdata))
			{
				if (i<BUFFER_SIZE) {
					buffer[i++] = pdata;
				}
			}
			//Watchdog::ResetCounter();

			slave.handler(buffer,i);
			i = 0;
			memset(buffer,0x00,sizeof(buffer));
			rs485.RecEnable(true);
		}
		//int rnd = getTrueRandomNumber();
	}
}

#pragma GCC diagnostic pop

#endif

// ----------------------------------------------------------------------------
