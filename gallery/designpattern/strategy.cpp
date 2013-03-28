// strategy.cpp

// The Strategy design pattern
#include <iostream>
using namespace std;

class NameStrategy
{
public:
	virtual void greet() = 0;
};

class SayHi : public NameStrategy
{
public:
	void greet()
	{
		cout << "Hil How's it going?" << endl;
	}
};

class Ignore : public NameStrategy
{
public:
	void greet()
	{
		cout << "(Pretend I don't see you)" << endl;
	}
};

class Admission : public NameStrategy
{
public:
	void greet()
	{
		cout << "I'm sorry. I forgot your name." << endl;
	}
};

// The "Context" controls the strategy:
class Context
{
	NameStrategy& strategy;
public:
	Context(NameStrategy& strat) : strategy(strat){}
	void greet() {strategy.greet();}
};

int main()
{
	SayHi sayhi;
	Ignore ignore;
	Admission admission;

	Context c1(sayhi), c2(ignore), c3(admission);
	c1.greet();
	c2.greet();
	c3.greet();
	return 0;
}