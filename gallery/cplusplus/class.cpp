// demonstration class construction/deconstruction, assign function etc.
// 

#include<iostream>
using namespace std;

class String
{
public:
	String(const char* data);
	~String();
	String(const String& rhs);
	String& operator=(const String& rhs);
	void Copy(const String& rhs);

private:
	char* m_data;
};

String::String(const char* data) 
	: m_data(NULL)
{
	m_data = NULL;
	if (data == NULL)
		return;

	m_data = new char[strlen(data)+1];
	strcpy(m_data, data);
}

String::~String()
{
	if (m_data)
	{
		delete[] m_data;
		m_data = NULL;
	}
}

String::String(const String& rhs)
	: m_data(NULL)
{
	m_data = NULL;
	if (rhs.m_data == NULL)
		return;

	m_data = new char[strlen(rhs.m_data)+1];
	strcpy(m_data, rhs.m_data);
}

String& String::operator=(const String& rhs)
{
	if (this == &rhs) 
		return *this;

	if (m_data) 
	{
		delete[] m_data;
		m_data = NULL;
	}
	m_data = new char[strlen(rhs.m_data)+1];
	strcpy(m_data, rhs.m_data);
	return *this;
}

void String::Copy(const String& rhs)
{
	if (rhs.m_data == NULL)
		return;
}

int main(int argc, char** argv)
{
	return (0);
};