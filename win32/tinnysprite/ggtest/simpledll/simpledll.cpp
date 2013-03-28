// simpledll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <stdio.h>
#include "interface.h"

class MySimpleDll : public SimpleDll{
public:
	MySimpleDll() {}
	~MySimpleDll() {}

	HRESULT SimpleMethod(){
		printf("MySimpleDll::SimpleMethod()\n");
		MessageBox(NULL, L"This hint from a dll module.", L"SimpleDll", MB_OK|MB_ICONWARNING);
		return S_OK;
	}

	HRESULT Swap(int& a, int& b){
		a = a + b;
		b = b - a;
		a = a - b;
		return S_OK;
	}
};

extern "C" void swap(int a, int b){
		a = a + b;
		b = a - b;
		a = a - b;
}

extern "C" void CreateInstance(SimpleDll* &pInstance)
{
	pInstance = new MySimpleDll;
}

extern "C" void DestoryInstance(SimpleDll* &pInstance)
{
	if (pInstance)
	{
		delete pInstance;
		pInstance = NULL;
	}
}

