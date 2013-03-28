/** 
 * 多重继承
 * 其语法格式：
 class 派生类名: 访问权限 基类名称,访问权限 基类名称,访问权限 基类名称
 {

 }
 */

// 初始顺序
// 初始化是按基类继承表中说明顺序进行初始的。

// 析构顺序
// 析构是按照基类继承表中的说明反顺序进行析构的。

#include<iostream>

class B1
{
public:
	B1()
	{
		std::cout<< "B1()" << std::endl;
	}
	~B1()
	{
		std::cout<< "~B1()" << std::endl;
	}
	void output()
	{
		std::cout<< "B1::output()" << std::endl;
	}
};

class B2
{
public:
	B2()
	{
		std::cout<< "B2()" << std::endl;
	}
	~B2()
	{
		std::cout<< "~B2()" << std::endl;
	}
	void output()
	{
		std::cout<< "B2::output()" << std::endl;
	}
};

class A : public B1, public B2
{
public:
	A()
	{
		std::cout<<"A()" << std::endl;
	}
	~A()
	{
		std::cout<<"~A()"<<std::endl;
	}
};

int main (int argc, char** argv)
{
	A a;
	a.output(); // error:访问output()方法不明确
};