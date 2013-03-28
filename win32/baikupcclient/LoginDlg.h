#pragma once

#include "Theme.h"
#include "Resource.h"
#include "QnSkinDialog.h"

class CLoginDlg : public QnSkinDialog
{
	DECLARE_DYNAMIC(CLoginDlg)
public:
	CLoginDlg(CWnd* pParent = 0);
	virtual ~CLoginDlg();

	enum {IDD = IDD_LOGIN};
	bool IsLogin() { return m_isLogin;}

protected:
	bool m_isLogin;

	CBitmap* m_pBmpLoginBg;

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	DECLARE_MESSAGE_MAP()
};