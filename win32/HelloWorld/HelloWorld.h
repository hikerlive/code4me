// HelloWorld.h : main header file for the HelloWorld application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CHelloWorldApp:
// See HelloWorld.cpp for the implementation of this class
//

class CHelloWorldApp : public CWinApp
{
public:
	CHelloWorldApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHelloWorldApp theApp;