// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

//#define DISABLE_LOGGER

//#include <afx.h>
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
//#ifndef _AFX_NO_OLE_SUPPORT
//#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
//#endif
//#ifndef _AFX_NO_AFXCMN_SUPPORT
//#include <afxcmn.h>                     // MFC support for Windows Common Controls
//#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>
// Windows Header Files:
#include <tchar.h>
#include <windows.h>
#include <process.h>
#include <time.h>

#ifndef DISABLE_LOGGER

#include "logger.h"

#else

#define DEFINE_LOGGER(name)
#define SSLOG_DEBUG(msg)
#define SSLOG_INFO(msg)
#define SSLOG_TRACE(msg)
#define SSLOG_ERROR(msg)

#endif

#pragma warning(disable:4231)
#pragma warning(disable:4996)
// TODO: reference additional headers your program requires here

#ifdef _DEBUG
    //#define DEBUG_CLIENTBLOCK   new(_CLIENT_BLOCK, __FILE__, __LINE__)
    #define DEBUG_CLIENTBLOCK   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
    #define DEBUG_CLIENTBLOCK
#endif // _DEBUG

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif

class StringSplit {
public:
    StringSplit(const char* str, const char* delimt);
    ~StringSplit();
    const char* next();
    bool eof();
private:
    char* str;
    char* delimt;
    char* p_next;
    char* p_this;
    char* p_new;
};

wchar_t* UTF8toWCHAR(wchar_t* buffer, size_t len, char* utf8);
#define UTF8TOWCHAR(utf8) \
    UTF8toWCHAR((wchar_t*)alloca((strlen(utf8)+1)*2), strlen(utf8)+1, (char*)utf8)

#if defined(_UNICODE) || defined(UNICODE)
#define UTF8TOTCHAR(utf8) UTF8TOWCHAR(utf8)
#else
#define UTF8TOTCHAR(utf8) (utf8)
#endif