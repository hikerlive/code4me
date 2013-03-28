#include<iostream>

// 发生函数覆盖的条件:
// 基类函数必须是虚函数(使用virtual关键字进行声明)
// 发生覆盖的两个函数必须要分别位于派生类和基类中
// 函数名称和参数列表必须完全相同

class Animal
{
public:
	Animal(){}
	~Animal(){}
	virtual void breath()
	{
		std::cout << "animal breath " << std::endl;
	}
};

class Fish
{
public:
	Fish(){}
	~Fish(){}
	void breath()
	{
		std::cout << "fish breath" << std::endl;
	}
};

int main (int argc, char** argv)
{
	return 0;
};
