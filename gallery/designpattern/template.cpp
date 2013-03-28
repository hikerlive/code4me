// template.cpp
// Simple demonstration of the template method design pattern

#include <iostream>
using namespace std;

class ApplicationFramework 
{
protected:
	virtual void custome1() = 0;
	virtual void custome2() = 0;
public:
	void templateMethod()
	{
		for (int i=0; i<5; i++)
		{
			custome1();
			custome2();
		}
	}
};

// Create a new 'application'
class MyApp : public ApplicationFramework
{
protected:
	void custome1(){cout << "Hello";}
	void custome2(){cout << "World!" << endl;}
};

int main()
{
	MyApp app;
	app.templateMethod();
	return 0;
}

