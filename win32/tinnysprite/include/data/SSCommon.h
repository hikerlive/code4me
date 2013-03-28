#ifndef __SSCOMMON_H__
#define __SSCOMMON_H__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
//#define USE_LOG_MODULE
#endif

#define VC_EXTRALEAN // Exclude rarely-used stuff from Windows headers
///////////////////////////////////////////////////////////////////////////////

#ifdef USE_MFC_MODULE
	#define _AFXDLL
	#include <afx.h>
	#include <afxwin.h> // MFC core and standard components
	#include <afxext.h> // MFC extensions
	#include <afxdtctl.h> // MFC support for Internet Explorer 4 Common Controls
	#ifndef _AFX_NO_AFXCMN_SUPPORT
	#include <afxcmn.h> // MFC support for Windows Common Controls
	#endif // _AFX_NO_AFXCMN_SUPPORT

	#ifdef _DEBUG
		#define MODULE_TRACE()  TRACE("%s(%d) : Module State nInst = 0x%X\n", \
		__FILE__, __LINE__, AfxGetModuleState()->m_hCurrentInstanceHandle)
		#define THREAD_TRACE()  TRACE("%s(%d) : Thread State Address = 0x%X\n",\
		__FILE__, __LINE__, AfxGetThreadState())
	#else
		#define MODULE_TRACE()
		#define THREAD_TRACE()
	#endif  //_DEBUG
#endif
///////////////////////////////////////////////////////////////////////////////
#include <string>
#include <assert.h>
#include <crtdbg.h>
using namespace std;

#include <winsock2.h>
//#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////
#ifdef USE_LOG_MODULE

#include <log4cpp/Category.hh>
using namespace log4cpp;

#include "SSLogModule.h"

#define SSTRACEMETHOD(msg)
//#define SSTRACEMETHOD(msg) SSMethodTraceLog mtl((msg))
#define SSTRACE(msg) SSLogModule::debug() << msg << log4cpp::eol

#else

#define SSTRACEMETHOD(msg)
#define SSTRACE(msg)

#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//#ifdef _DEBUG
//   #define DEBUG_CLIENTBLOCK   new(_CLIENT_BLOCK, __FILE__, __LINE__)
//#else
//   #define DEBUG_CLIENTBLOCK
//#endif // _DEBUG
//
//#ifdef _DEBUG
//#define new DEBUG_CLIENTBLOCK
//#endif

///////////////////////////////////////////////////////////////////////////////

#include "SSConcurrency.h"

#endif