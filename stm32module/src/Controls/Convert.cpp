#include <stdlib.h>
#include <locale.h>
#include "Convert.h"

int Convert::ToInt(CString arg)
{
	int result;
	const char* pchar = arg.c_str();
	result = atoi(pchar);
	delete[] pchar;
	return result;
}

float Convert::ToFloat(CString arg)
{
	float result;
	const char* pchar = arg.c_str();
	result = atof(pchar);
	delete[] pchar;
	return result;
}
