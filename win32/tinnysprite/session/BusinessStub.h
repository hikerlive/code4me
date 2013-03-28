// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BUSINESSSTUB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BUSINESSSTUB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

//#ifdef BUSINESSSTUB_EXPORTS
//#define BUSINESSSTUB_API __declspec(dllexport)
//#else
//#define BUSINESSSTUB_API __declspec(dllimport)
//#endif
//
//// This class is exported from the BusinessStub.dll
//class BUSINESSSTUB_API CBusinessStub {
//public:
//	CBusinessStub(void);
//	// TODO: add your methods here.
//};
//
//extern BUSINESSSTUB_API int nBusinessStub;
//
//BUSINESSSTUB_API int fnBusinessStub(void);
