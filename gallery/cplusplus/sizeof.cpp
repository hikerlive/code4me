// sizeof.cpp

#include <iostream>
using namespace std;

void method1(char str[100])
{
	cout << sizeof(str) << endl;
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
	char	str[] = {"Hello"};
	char*	p = str;
	void*	p1 = malloc(100);
	int		n = 10;

	char	a[30] = {0};
	cout << sizeof(a) << endl;
	cout << sizeof(*(a+3)) << endl;

	cout << sizeof(str) << endl;
	cout << strlen(str) << endl;
	cout << sizeof(p) << endl;
	cout << sizeof(p1) << endl;
	cout << sizeof(n) << endl;

	// case 2:
	union Data{long a; int b[5]; char c;};
	cout << sizeof(Data) << endl;
	return 0;
};
