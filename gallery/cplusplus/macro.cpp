// macro.cpp
// Implements macro define usage.

#include <iostream>
using namespace std;

#define DOUBLE(x) x+x
#define DOUBLE2(x) (x+x)
#define COUNTF(array) (sizeof(x)/sizeof(x[0]))

int main(int argc, char** argv)
{
	int i = 0;
	i = 5*DOUBLE(5);
	cout << i << endl; // surprise here, i = 30.

	int j=0;
	j = 5*DOUBLE2(5);
	cout << j << endl; // surprise here, j = 50.
	return (0);
}