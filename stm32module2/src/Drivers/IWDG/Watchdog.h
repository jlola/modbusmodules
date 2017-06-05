/*
 * Watchdog.h
 *
 *  Created on: 10. 1. 2017
 *      Author: pc
 */

#ifndef DRIVERS_IWDG_WATCHDOG_H_
#define DRIVERS_IWDG_WATCHDOG_H_

class Watchdog {
	Watchdog();
public:
	static void Start();
	static void ResetCounter();
	virtual ~Watchdog();
};

#endif /* DRIVERS_IWDG_WATCHDOG_H_ */
