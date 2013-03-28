// stdafx.cpp : source file that includes just the standard includes
// BusinessStub.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <string.h>
// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
StringSplit::StringSplit(const char* str, const char* delimt) {
    this->str = (char*)str;
    this->delimt = (char*)delimt;
    this->p_new = new char[strlen(str)+1];
    strcpy(this->p_new, str);
    this->p_next = this->p_new;
    this->p_this = this->p_new;
}
StringSplit::~StringSplit() {
    if (this->p_new != NULL) {
        delete this->p_new;
        this->p_new = NULL;
    }
}
const char* StringSplit::next() {
    p_this = p_next;
    p_next = (char*)strstr(p_this, delimt);
    if (p_next != NULL) {
        *p_next = '\0';
        p_next += strlen(delimt);
    }
    return p_this;
}
bool StringSplit::eof() {
    if (p_next == NULL || p_next[0] == 0)
        return true;
    return false;
}


wchar_t* UTF8toWCHAR(wchar_t* buffer, size_t len, char* utf8)
{
	if (utf8 == NULL)
        return NULL;
	// verify that no illegal character present
	// since lpw was allocated based on the size of lpa
	// don't worry about the number of chars
	*buffer = '\0';
	int ret = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, buffer, len);
	if(ret == 0)
	{
		return NULL;
	}
	return buffer;
}