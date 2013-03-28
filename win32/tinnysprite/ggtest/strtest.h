#ifndef	_TEST_STR_H
#define _TEST_STR_H

class StrTest
{
public:
	StrTest(const char* str);
	~StrTest();

	const char* str() { return _str; }

private:
	char* _str;
};

#endif //_TEST_STR_H

