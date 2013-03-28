// factorypoly.cpp

// Polymorphic factory Methods.
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class Shape
{
public:
	virtual void draw() = 0;
	virtual void erase() = 0;
	virtual ~Shape(){}
};

class ShapeFactory
{
	virtual Shape* create() =0;
	static map<string, ShapeFactory*> factories;
public:
	virtual ~ShapeFactory(){}
	friend class ShapeFactoryInitializer;
	static Shape* createSahpe(const string& id)
	{
		if (factories.find(id) != factories.end())
			return factories[id]->create();
		else 
			throw;
	}
};

// Define the static object:
map<string, ShapeFactory*> ShapeFactory::factories;

class Circle : public Shape
{
	Circle() {} // Private constructor
	friend class ShapeFactoryInitializer;
	class Factory;
	friend class Factory;
	class Factory : public ShapeFactory
	{
	public:
		Shape* create() { return new Circle;}
		friend class ShapeFactoryInitializer;
	};
};