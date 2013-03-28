#include "stdafx.h"
#include "BaikuDemo.h"
#include "LoginDlg.h"

#pragma warning(disable:4244)

IMPLEMENT_DYNAMIC(CLoginDlg, QnSkinDialog)

CLoginDlg::CLoginDlg(CWnd* parent) 
	: QnSkinDialog(CLoginDlg::IDD, parent)
{
	SetTheme(theApp.m_pTheme);
	CString strPath = theApp.GetAppResPath() + _T("\\title_logo.png");
	m_pIconImage = LoadPngImage(strPath);
}

CLoginDlg::~CLoginDlg()
{
	if (m_pBmpLoginBg) {
		delete m_pBmpLoginBg;
		m_pBmpLoginBg = 0;
	}
}

BEGIN_MESSAGE_MAP(CLoginDlg, QnSkinDialog)
END_MESSAGE_MAP()

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	QnSkinDialog::DoDataExchange(pDX);
}

BOOL CLoginDlg::OnInitDialog()
{
	QnSkinDialog::OnInitDialog();
	
	CString strPath = theApp.GetAppResPath() + _T("\\login_bg.bmp");
	m_pBmpLoginBg = LoadBmpImage(strPath);

	if (m_pBmpLoginBg) {
		// 创建绘制的背景的DC
		CreateQnDC(m_pBmpLoginBg, true);
	}
	
	return TRUE;
}
