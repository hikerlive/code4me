#ifndef STRUTIL_H
#define STRUTIL_H

#include <string>
#include <tchar.h>

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> tstring;

int utf8_wchar(const std::string &utf8, std::wstring &wide);
int wchar_utf8(const std::wstring &wide, std::string &utf8);
std::string utf82ansi(const std::string& str);
std::string ansi2utf8(const std::string& str);

std::string w2a(const std::wstring& w);
std::wstring a2w(const std::string& a);
std::wstring utf82wchar(const std::string& a);
const wchar_t * c2wc(const char* c);

#if defined(_UNICODE) || defined(UNICODE)
#define a2t(s) a2w(s)
#define w2t(s) s
#define t2a(s) w2a(s)
#define t2w(s) s
#else
#define a2t(s) s
#define w2t(s) w2a(s)
#define t2a(s) s
#define t2w(s) a2w(s)
#endif

#endif