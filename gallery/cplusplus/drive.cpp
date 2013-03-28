#include <iostream>

class Animal
{
public:
	Animal(int a, int b){}
	~Animal(){}
};

class Fish : public Animal
{
public:
	
	// 原因分析:子类提供默认构造函数，但是基类没有提供默认构造函数。
	// 子类在进行对象实例化时会调用基类的默认构造函数，所以才会有这个错误出现。
	// Fish(){}; // error C2512: “Animal”: 没有合适的默认构造函数可用
	
	// 解决方法:可以在子类中指定调用基类带参数的构造函数。 
	Fish() : Animal(2, 3){}
	~Fish(){};
};

/**
 * 注意：
 * 基类中的private成员不能子类所访问，因此基类中的private成员不能被子类继承。
 */

// 默认是private继承
// 基类所有成员都变成子类的private成员
class Cat : Animal
{
public:
	Cat() : Animal(2, 3){}
	~Cat();
};

// protected继承
// 基类中public和protected都变成子类的protected成员
class Bird : public Animal
{
public:
	Bird() : Animal(2,3){}
	~Bird();
};

// public继承
// 基类中public和protected成员仍以原有权限属性变成子类成员
class Dog : public Animal
{
public:
	Dog() : Animal(2, 3){}
	~Dog(){}
};



int main(int argc, char** argv)
{
	Fish fish; 
	return 0;
};