#include "strutil.h"
#include <stdlib.h>
#include <windows.h>
#include "ConvertUTF.h"
#include <string.h>

int utf8_wchar(const std::string &utf8, std::wstring &wide)
{
	// allocate space for worst-case
	wide.resize(utf8.size());
	wchar_t const* dst_start = wide.c_str();
	char const* src_start = utf8.c_str();
	ConversionResult ret;
	if (sizeof(wchar_t) == sizeof(UTF32))
	{
		ret = ConvertUTF8toUTF32((const UTF8**)&src_start, (const UTF8*)src_start
			+ utf8.size(), (UTF32**)&dst_start, (UTF32*)dst_start + wide.size()
			, lenientConversion);
		wide.resize(dst_start - wide.c_str());
		return ret;
	}
	else if (sizeof(wchar_t) == sizeof(UTF16))
	{
		ret = ConvertUTF8toUTF16((const UTF8**)&src_start, (const UTF8*)src_start
			+ utf8.size(), (UTF16**)&dst_start, (UTF16*)dst_start + wide.size()
			, lenientConversion);
		wide.resize(dst_start - wide.c_str());
		return ret;
	}
	else
	{
		return sourceIllegal;
	}
}

int wchar_utf8(const std::wstring &wide, std::string &utf8)
{
	// allocate space for worst-case
	utf8.resize(wide.size() * 6);
	if (wide.empty()) return 0;
	char* dst_start = &utf8[0];
	wchar_t const* src_start = wide.c_str();
	ConversionResult ret;
	if (sizeof(wchar_t) == sizeof(UTF32))
	{
		ret = ConvertUTF32toUTF8((const UTF32**)&src_start, (const UTF32*)src_start
			+ wide.size(), (UTF8**)&dst_start, (UTF8*)dst_start + utf8.size()
			, lenientConversion);
		utf8.resize(dst_start - &utf8[0]);
		return ret;
	}
	else if (sizeof(wchar_t) == sizeof(UTF16))
	{
		ret = ConvertUTF16toUTF8((const UTF16**)&src_start, (const UTF16*)src_start
			+ wide.size(), (UTF8**)&dst_start, (UTF8*)dst_start + utf8.size()
			, lenientConversion);
		utf8.resize(dst_start - &utf8[0]);
		return ret;
	}
	else
	{
		return sourceIllegal;
	}
}

std::string w2a(const std::wstring& w) {
    std::string ret;
    int result = wchar_utf8(w, ret);
    if (result == 0) return ret;

    ret.clear();
    const wchar_t* end = &w[0] + w.size();
    for (const wchar_t* i = &w[0]; i < end; i++)
    {
	    char c[20] = "";
	    int result = std::wctomb(c, *i);
	    if (result > 0) {
            c[result] = '\0';
            ret += c;
        } else {
            break;
        }
    }
    return ret;
}

std::wstring a2w(const std::string& a) {
    std::wstring ret;
    int result = utf8_wchar(a, ret);
    if (result == 0) return ret;

    ret.clear();
    const char* end = &a[0] + a.size();
    for (const char* i = &a[0]; i < end;)
    {
	    wchar_t c = L'\0';
	    int result = std::mbtowc(&c, i, end - i);
	    if (result > 0) i += result;
	    else ++i;
	    ret += c;
    }
    return ret;
}

std::string utf82ansi(const std::string& str)
{ 
	const char* pszUtf8 = str.c_str();
	int nWideLen = MultiByteToWideChar(CP_UTF8, 0, pszUtf8, strlen(pszUtf8), 0, 0); 
	wchar_t * szWideForm = new wchar_t[nWideLen + 1]; 
	MultiByteToWideChar(CP_UTF8, 0, pszUtf8, strlen(pszUtf8), szWideForm, nWideLen); 
	szWideForm[nWideLen] = 0; 

	int nAnsiLen = WideCharToMultiByte(CP_ACP, 0, szWideForm, nWideLen, 0, 0, NULL, NULL); 
	char * szAnsi = new char[nAnsiLen + 1]; 
	WideCharToMultiByte(CP_ACP, 0, szWideForm, nWideLen, szAnsi, nAnsiLen, NULL, NULL); 
	szAnsi[nAnsiLen] = 0; 
	delete[] szWideForm; 
	std::string ansi = szAnsi;
	delete[] szAnsi;
	return ansi; 
}

std::string ansi2utf8(const std::string& str)
{ 
	const char* pszAnsi = str.c_str();
	int nWideLen = MultiByteToWideChar(CP_ACP, 0, pszAnsi, strlen(pszAnsi), 0, 0); 
	wchar_t * szWideForm = new wchar_t[nWideLen + 1]; 
	MultiByteToWideChar(CP_ACP, 0, pszAnsi, strlen(pszAnsi), szWideForm, nWideLen); 
	szWideForm[nWideLen] = 0; 

	int nUtf8Len = WideCharToMultiByte(CP_UTF8, 0, szWideForm, nWideLen, 0, 0, NULL, NULL); 
	char * szUtf8 = new char[nUtf8Len + 1]; 
	WideCharToMultiByte(CP_UTF8, 0, szWideForm, nWideLen, szUtf8, nUtf8Len, NULL, NULL); 
	szUtf8[nUtf8Len] = 0; 
	delete[] szWideForm; 
	std::string utf8 = szUtf8;
	delete[] szUtf8;
	return utf8; 
}


std::wstring utf82wchar(const std::string& a)
{
	return a2t(utf82ansi(a));
}

const wchar_t * c2wc(const char* c){
	return a2t(std::string(c)).c_str();
}