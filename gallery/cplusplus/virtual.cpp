#include <iostream>

class Animal
{
public:	
	Animal(){};
	~Animal(){};
	virtual void breath()
	{
		std::cout << "animal breath" << std::endl;
	}

	// 重载构成的条件:函数参数类型、参数个数不同、才能构成重载。
	// 只有函数返回值不同是不能够构成的。
	void move();	
	void move(int a, int b);

	// 以下这种情况是不能构成函数的重载，调用有歧义。
	void add(int a){};
	void add(int a, int b=5){};
};

class Fish : public Animal
{
public:
	Fish(){};
	~Fish(){};
	void breath() // 重写基类的函数
	{
		std::cout << "fish breath" << std::endl;
	}
};

int main(int argc, char* argv[])
{
	Fish fish;
	Animal* pAn = &fish;
	pAn->breath(); // call class fish::breath()	
	return 0;
}

