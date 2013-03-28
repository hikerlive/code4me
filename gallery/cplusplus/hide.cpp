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
	void breath()
	{
		std::cout<<"animal breath"<<std::endl;
	}
	virtual void add(int a, int b)
	{

	}
};

class Fish : public Animal
{
public:
	Fish(){}
	~Fish(){}
	void breath() //隐藏基类的breath函数,原因基类的breath函数不是虚函数
	{
		std::cout<<"fish breath"<<std::endl;
	}

	// 派生类具有基类同名函数但是不同参数列表的函数,不管基类的函数声明是否有
	// virtual关键字,基类函数都将被隐藏.
	void add(int a, int b, int c)
	{
		std::cout<<"fish add()"<<std::endl;
		// 如果有需要可以在此调用基类的同名但不同参数列表的函数.
		Animal::add(a, b);
	}
};


// 覆盖发生在基类和派生类之间,且基类函数必须是虚函数.
// 除此之外均是隐藏
int main (int argc, char** argv)
{
	Fish fish;
	fish.breath();
	fish.add(1,2,3);
	return 0;
};
