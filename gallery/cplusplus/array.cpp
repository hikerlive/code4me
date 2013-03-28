// array.cpp

#include <iostream>
using namespace std;

void method1(void)
{
	int		array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int		* p = array;
	int		i = 0;

	cout << sizeof(array)<< endl;
	
	// printf array elements: 
	for (i=0; i<sizeof(array)/sizeof(int); i++)
	{
		cout << *p++ << endl;
	}
}

void method2(void)
{
	int		i=0;
	char*	str[] = {"ADD", "SUB", "MUL", "DIV"};
	char*	p = *str;
	
	cout << sizeof(str) << endl;
	for ( i=0; i<sizeof(str)/sizeof(char*); i++)
	{
		cout << str[i] << ":" << sizeof(str[i]) << "," << strlen(str[i]) << endl;
	}	
}

void method3(void)
{
	struct Item{
		int		ival;
		double	dval;
		bool	flag;
		char*	argv;
	};

	Item items[] = {
		{0, false, "FAILED"},
		{1, true,  "SUCCED"}
	};
	
	// sizeof(Item)时出现边界对齐问题，所以sizeof(Item) = 12. 
	cout << sizeof(items) << "\tstruct size:" << sizeof(Item) <<endl;
}

int main(int argc, char** argv)
{
	method2();
	method3();
	return 0;
}