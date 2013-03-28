#ifndef __STRING_H
#define __STRING_H

#include <string.h>

class String {
public: 
	String(const char* str=NULL);
    String(const String& rhs);
	String& operator=(const String& rhs);
	~String();
private:
	char* m_data;
};

#endif /*__STRING_H */