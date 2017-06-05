/*
 * Watchdog.cpp
 *
 *  Created on: 10. 1. 2017
 *      Author: pc
 */

#include "stm32f0xx_iwdg.h"
#include "IWDG/Watchdog.h"


Watchdog::Watchdog() {
	// TODO Auto-generated constructor stub

}

void Watchdog::Start()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	while(!IWDG_GetFlagStatus(IWDG_FLAG_PVU));
	IWDG_SetReload(0x0FFF);
	while(!IWDG_GetFlagStatus(IWDG_FLAG_RVU));
	IWDG_Enable();
}

void Watchdog::ResetCounter()
{
	IWDG_ReloadCounter();
}

Watchdog::~Watchdog() {
	// TODO Auto-generated destructor stub
}

