
// BaikuDemo.h : main header file for the BaikuDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "Theme.h"

// CBaikuDemoApp:
// See BaikuDemo.cpp for the implementation of this class
//

class CBaikuDemoApp : public CWinAppEx
{
public:
	CBaikuDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	ULONG_PTR m_gdiplusToken;
	CString m_strAppPath;
	CString m_strAppResPath;
	ThemeDialog* m_pTheme;

	void LoadTheme();
	void UnloadTheme();
	CString GetAppPath();
	CString GetAppResPath();
protected:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBaikuDemoApp theApp;
