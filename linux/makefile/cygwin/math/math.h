#ifndef __MATH_H
#define __MATH_H

#include <stdio.h>

int add(int i, int j);
int sub(int i, int j);
int mul(int i, int j);
int div(int i, int j);

class HKMath
{
public:
	HKMath(int i, int j);
	~HKMath();

	int Add(int i, int j);
	int Sub(int i, int j);
	int Mul(int i, int j);
	int Div(int i, int j);

private:
	int i;
	int j;
};

#endif
