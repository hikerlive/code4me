// **************************************************************************
// shellnotify.h
//
// Description:
//	Defines a shell notify class, add a icon in windows shell notify district.
//
// DISCLAIMER:
//	Copyright (c) BeiJing Dafeng software Tech.CO.,LTD. 2010
// **************************************************************************

#ifndef __SHELLNOTIFY_H
#define __SHELLNOTIFY_H

class CShellNotify
{
public:
	CShellNotify();
	~CShellNotify();

	static void Create(UINT menuID);
	static void Destory();
	static long WINAPI ShellNotifyProc(WPARAM wParam, LPARAM lParam);
private:
	static CMenu m_nidTrayMenu;
	static NOTIFYICONDATA m_nidIconData;
};

// system trayicon messages
#define WM_SHELLNOTIFYICON	(WM_USER + 2000)

#endif //__SHELLNOTIFY_H