// BusinessStub.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "BusinessStub.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// The one and only application object

//CWinApp theApp;
//
//using namespace std;
//
//int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
//{
//	int nRetCode = 0;
//
//	HMODULE hModule = ::GetModuleHandle(NULL);
//
//	if (hModule != NULL)
//	{
//		// initialize MFC and print and error on failure
//		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
//		{
//			// TODO: change error code to suit your needs
//			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
//			nRetCode = 1;
//		}
//		else
//		{
//			// TODO: code your application's behavior here.
//		}
//	}
//	else
//	{
//		// TODO: change error code to suit your needs
//		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
//		nRetCode = 1;
//	}
//
//	return nRetCode;
//}

//#include <fcntl.h>
//#include <io.h>
//
//class CBusinessStubApp : public CWinApp {
//public:
//    virtual BOOL InitInstance();
//    virtual int ExitInstance();
//} theApp;
////#include "logger.h"
////static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger(_T("stub.app")));
//
//BOOL CBusinessStubApp::InitInstance() {
//    AllocConsole();
//    freopen("CONIN$", "r", stdin);
//    freopen("CONOUT$", "w", stderr);
//    freopen("CONOUT$", "w", stdout);
//
//    //int hCrt;
//    //FILE *hf;
//    //hCrt = _open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
//    //hf = _fdopen( hCrt, "w" );
//    //*stdout = *hf;
//    //int i = setvbuf( stdout, NULL, _IONBF, 0 );
//
//    printf("CBusinessStubApp::InitInstance");
//    //LOG4CXX_TRACE(logger, _T("CBusinessStubApp::InitInstance"));
//    return TRUE;
//}
//
//int CBusinessStubApp::ExitInstance() {
//    FreeConsole();
//    return 0;
//}
