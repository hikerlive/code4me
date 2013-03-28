// **************************************************************************
// shellnotify.cpp
//
// Description:
//	Defines a shell notify class, add a icon in windows shell notify district.
//
// DISCLAIMER:
//	Copyright (c) BeiJing Dafeng software Tech.CO.,LTD. 2010
// **************************************************************************

#include "stdafx.h"
#include "mainframe.h"
#include "shellnotify.h"

// private static class memeber defines:
CMenu CShellNotify::m_nidTrayMenu;
NOTIFYICONDATA CShellNotify::m_nidIconData;

CShellNotify::CShellNotify()
{

}

CShellNotify::~CShellNotify()
{

}

// **************************************************************************
// Create ()
//
// Description:
//	Create a shell icon in windows shelltray district, and only create once.
//	Need call it when application initialize.
//
// Parameters:
//	none
//	
// Returns:
//  none
// **************************************************************************
void CShellNotify::Create(UINT menuID)
{
	// Add system tray icon for application.
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ASSERT(hIcon != NULL);
	if (hIcon)
	{
		m_nidIconData.cbSize			= sizeof(m_nidIconData);
		m_nidIconData.uCallbackMessage	= WM_SHELLNOTIFYICON;
		m_nidIconData.uFlags			= NIF_MESSAGE;
		m_nidIconData.hWnd				= AfxGetMainWnd()->m_hWnd;
		m_nidIconData.uID				= 1;
		m_nidIconData.hIcon				= hIcon;
		m_nidIconData.uFlags			|= NIF_ICON;
		m_nidIconData.uFlags			|= NIF_TIP;
		strcpy(m_nidIconData.szTip, _T("Dafeng OPC Server"));

		m_nidTrayMenu.LoadMenu(menuID);
		Shell_NotifyIcon(NIM_ADD, &m_nidIconData);
	}
}

// **************************************************************************
// Destory ()
//
// Description:
//	Destory shell icon in windows shelltray district.
//	Need call it when application exitinstance.
//
// Parameters:
//	none
//
// Returns:
//  none
// **************************************************************************
void CShellNotify::Destory()
{
	Shell_NotifyIcon(NIM_DELETE, &m_nidIconData);
}

// **************************************************************************
// ShellNotifyProc ()
//
// Description:
//	Shell icon notify procedure, include window mouse click, right-click
//	message etc. you don't need to change this code.
//
// Parameters:
//
// Returns:
//  none
// **************************************************************************
long WINAPI CShellNotify::ShellNotifyProc(WPARAM wParam, LPARAM lParam)
{
	UINT uID;
	UINT uMessage;

	uID = (UINT) wParam;
	uMessage = (UINT) lParam;

	if (uID != 1 ) return (FALSE);

	CPoint pt;

	CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();
	ASSERT(pMainFrame != NULL);

	switch(uMessage) 
	{
	case WM_MOUSEMOVE:
		GetCursorPos(&pt);
		pMainFrame->ClientToScreen(&pt);
		break;
		
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		GetCursorPos(&pt);
		pMainFrame->ClientToScreen(&pt);
		pMainFrame->ShowWindow(SW_SHOWNORMAL);
		pMainFrame->SetForegroundWindow();
		break;
	
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
		GetCursorPos(&pt);
		m_nidTrayMenu.GetSubMenu(0)->SetDefaultItem(0, TRUE);
		m_nidTrayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN
			|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
			pt.x,pt.y,
			pMainFrame);
		break;

	case WM_RBUTTONDBLCLK:
		GetCursorPos(&pt);
		AfxGetMainWnd()->ClientToScreen(&pt);
		m_nidTrayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN
			|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
			pt.x,pt.y,
			pMainFrame);
		break;
		
	default:
		break;
	}
	
	return (TRUE);
}