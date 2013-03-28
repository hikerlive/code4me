// string.cpp

#include <assert.h>
#include <iostream>
using namespace std;

char* xstrcpy(char* strDest, const char* strSrc)
{
	assert((strDest != NULL) && (strSrc != NULL));
	char*	p = strDest;
	while ((*strDest++ = *strSrc++) != '\0')
		NULL;
	return p;
}

int xstrlen(char* strDest)
{
	assert((strDest != NULL));
	int count = 0;
	while (*strDest++ != '\0')
		count ++;
	return count;
}

char* xfoo(char* strDest)
{
	assert((strDest != NULL));
	char*	p = strDest;
	while ((*strDest++) != '\0')
		NULL;
	return p;
}

char* xfoo1(char** strDest)
{
	assert((strDest != NULL));
	char*	p = *strDest;
	while ((*(*strDest)++) != '\0')
		NULL;
	return p;
}

char* GetString1(void)
{
	char str[] = {"Hello World"};
	return str; // error: return stack memory:
}

char* GetString2(void)
{
	char *p = "Hello World";
	return p; // right: return static stack memory(静态存储区,是只读的)
}

char* GetString3(int num)
{
	char* p = (char*)malloc(sizeof(char) * num);
	return p; // right: return dynamic memory(动态内存,返回后需释放)
}

void GetString4(char* str, int num)
{
	str = (char*)malloc(sizeof(char) * num);
	return;  // error: str在调用者堆栈中仍是NULL,并不能传回申请的动态内存.
}

void GetString5(char** str, int num)
{
	*str = (char*)malloc(sizeof(char) * num);
	return; // right: *str是内容,是可以被改变并且可以传出来的.
}

void GetMemory(char* p, int num)
{
	//重要:
	//	编译器总是要为函数的每个参数制作临时副本,指针参数p的副本是_p,
	//  编译器使_p = p; 如果函数体内修改了_p的内容,就导致参数p的内容作相应修改.
	//	这就是指针作为输出参数的原因.

	//分析:
	//	在本例中,_p申请了新的内存,只是把_p所指的内存地址改变了,但是p丝毫改变。
	//  所以GetMemory并不能输出任何东西。
	p = (char*)malloc(sizeof(char)*num);
	return;
}

int main (int argc, char** argv)
{
	// step1:
	char str1[64] = {0};
	char str2[32] = {"Hello"};
	xstrcpy(str1, str2);
	cout << str1 << endl;
	cout <<xstrlen(str1)<<endl;

	char*	p	= xfoo(str1); // surprise: str1: Hello, no ""
	cout << str1 << endl;

	// step2:

};