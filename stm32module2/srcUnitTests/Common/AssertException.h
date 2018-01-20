/*
 * AssertException.h
 *
 *  Created on: 27. 11. 2017
 *      Author: Libor
 */

#ifndef ASSERTEXCEPTION_H_
#define ASSERTEXCEPTION_H_

#include <stdlib.h>
#include <string>

using namespace std;

class AssertException : protected exception
{
	string message;
public:
	AssertException(int line,string file);

	const char* what();

	~AssertException();
};

#endif /* ASSERTEXCEPTION_H_ */
