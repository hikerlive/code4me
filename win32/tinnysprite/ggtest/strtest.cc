#include "strtest.h"
#include <stdio.h>
#include <string.h>

StrTest::StrTest(const char* str)
{
	if (str != NULL)
	{
		int len = strlen(str);
		_str = new char[len+1];
		strcpy(_str, str);
		_str[len] = '\0';
	}
	else
	{
		_str = new char[1];
		_str[0] = '\0';
	}
}

StrTest::~StrTest()
{
	if (_str)
		delete _str;
	_str = NULL;
}
