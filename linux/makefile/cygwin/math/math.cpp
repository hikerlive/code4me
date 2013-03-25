#include "math.h"

int add(int i, int j)
{
    return i+ j;
}

int sub(int i, int j)
{
    return i-j;
}

int mul(int i, int j)
{
    return i*j;
}

int div(int i, int j)
{
    return (i/j);
}

/////////////////////////////////////////////////////////////////////////////
// HKMath
HKMath::HKMath(int i, int j)
{

}

HKMath::~HKMath()
{

}

int HKMath::Add(int i, int j)
{
	return i + j;
}

int HKMath::Sub(int i, int j)
{
	return i-j;
}

int HKMath::Mul(int i, int j)
{
	return i - j;
}

int HKMath::Div(int i, int j)
{
	return  (j) ? i/j : 0;
}
