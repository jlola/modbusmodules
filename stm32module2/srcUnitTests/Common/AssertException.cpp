/*
 * AssertException.cpp
 *
 *  Created on: 27. 11. 2017
 *      Author: Libor
 */

#include <string>
#include <AssertException.h>
#include <stdlib.h>

AssertException::AssertException(int line, string file) {
	this->message = file.c_str();
	printf("Assert exception in file %s at line %d\n",file.c_str(),line);
}


const char* AssertException::what()
{
	return message.c_str();
}

AssertException::~AssertException() {
}

