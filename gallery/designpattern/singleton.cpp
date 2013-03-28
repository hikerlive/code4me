// singleton.cpp

#include <iostream>
using namespace std;

class Singleton
{
	static Singleton s;
	int i;
	Singleton(){}
	Singleton(int x) : i(x){} // Disallowed
	Singleton(const Singleton& si){} // Disallowed
	Singleton& operator=(const Singleton& si){} // Disallowed
public:
	static Singleton& instance(){ return s;}
	int getValue() { return i;}
	void setValue(int x) { i = x; }
	friend ostream& operator<<(ostream& os, const Singleton& si)
	{
		os << si.i << endl;
		return os;
	}
};
Singleton Singleton::s(7);




















int main(int argc, char** argv)
{
	Singleton& s1 = Singleton::instance();
	cout << s1.getValue() << endl;
	s1.setValue(9);

	Singleton& s2 = Singleton::instance();
	cout << s2.getValue() << endl;

	cout << s2;
	return 0;
}

