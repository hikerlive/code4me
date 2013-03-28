#ifndef INCLUDED_XSTRING_H
#define INCLUDED_XSTRING_H

#pragma warning(disable:4786)
#include <windows.h>
#include <string>
#include <vector>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
extern char *		strtolower(char * str);
extern char *		hexstrdup(unsigned char const * src);
extern unsigned int	hexstrtoraw(unsigned char const * src, char * data, unsigned int datalen);
extern unsigned char	xtoi(unsigned char ch);
extern char * *		strtoarray(char const * str, char const * delim, int * argc);
extern char * *		strtoargv(char const * str, int * argc);
extern char *		arraytostr(char * * array, char const * delim,int count);
extern char *		str_strip_affix(char * str, char const * affix);
#ifdef __cplusplus
}
#endif

class XStringToken
{
public:
	XStringToken(LPCSTR szString);
	XStringToken();
	~XStringToken();
	void SetString(LPCSTR szString);
	char const * GetString() {
		return str.c_str();
	}
	BOOL ToArgv();
	BOOL ToArray(char const * delim);
	DWORD size() {
		return array.size();
	}
	char const * operator[] (int n) const {
		return array[n].c_str();
	}

private:
	vector<string>	array;
	string			str;
};

#endif
