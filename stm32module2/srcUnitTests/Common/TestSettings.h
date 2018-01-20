/*
 * TestSettings.h
 *
 *  Created on: 27. 11. 2017
 *      Author: Libor
 */

#ifndef TESTSETTINGS_H_
#define TESTSETTINGS_H_

#ifdef UNITTEST

#include "gtest/gtest.h"
#include "AssertException.h"

using namespace std;

#undef assert_param
#define assert_param(e)  (e) ? (void)0 : throw AssertException(__LINE__,__FILE__);

#endif

#endif /* TESTSETTINGS_H_ */
