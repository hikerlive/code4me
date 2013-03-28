#include "stdafx.h"
#include "QnBaseDialog.h"

IMPLEMENT_DYNAMIC(QnBaseDialog, CDialogEx)

QnBaseDialog::QnBaseDialog(UINT uiid, CWnd* parent) :
	CDialogEx(uiid, parent),
	m_pBgDc(0)
{
}

QnBaseDialog::~QnBaseDialog()
{
}

BEGIN_MESSAGE_MAP(QnBaseDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void QnBaseDialog::CreateQnDc(UINT nBmpResId, bool bStretch)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(nBmpResId);
	CreateQnDC(&bitmap, bStretch);
}
void QnBaseDialog::CreateQnDC(CBitmap* pBmp, bool bStretch)
{
	m_pBgDc = new QnDC();
	CDC* pDC = GetDC();
	m_pBgDc->CreateFromDC(pDC, this);
	if (bStretch) {
		m_pBgDc->DrawStretchBitmap(pBmp);
	} else {
		m_pBgDc->DrawFillBitmap(pBmp);
	}
	ReleaseDC(pDC);
}
void QnBaseDialog::OnClose()
{
	CDialogEx::OnClose();
	if (m_pBgDc) {
		delete m_pBgDc;
		m_pBgDc = 0;
	}
}
void QnBaseDialog::OnPaint()
{
	CPaintDC dc(this);
	if (m_pBgDc) {
		m_pBgDc->DrawToDC(&dc, this);
	}
}
void QnBaseDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}
BOOL QnBaseDialog::OnEraseBkgnd(CDC* pDC)
{
	if (m_pBgDc) {
		return TRUE;
	}
	return CDialogEx::OnEraseBkgnd(pDC);
}
HBRUSH QnBaseDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// 更改DC属性
	HBRUSH hBrush = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// 有背景图片，有一些控件显示会有空白区，所以得设置其为透明。
	if (nCtlColor == CTLCOLOR_STATIC) {	
		pDC->SetBkMode(TRANSPARENT);
		if (m_pBgDc) {
			// 释放掉引用
			HGDIOBJ hObj = m_bgBrush.Detach();
			if (hObj) {
				::DeleteObject(hObj);
			}

			// 获取当前窗体区域图像
			HBITMAP hBitmap = m_pBgDc->GetBitmap(pWnd);
			m_bgBrush.CreatePatternBrush(CBitmap::FromHandle(hBitmap));
			::DeleteObject(hBitmap);
			return m_bgBrush;
		}
	}
	if (nCtlColor == CTLCOLOR_BTN) {
		if (m_pBgDc) {
			HGDIOBJ hObj = m_bgBrush.Detach();
			if (hObj) {
				::DeleteObject(hObj);
			}
			// 获取当前窗体区域图像
			HBITMAP hBitmap = m_pBgDc->GetBitmap(pWnd);
			m_bgBrush.CreatePatternBrush(CBitmap::FromHandle(hBitmap));
			::DeleteObject(hBitmap);
			return m_bgBrush;
		}
	}
	return hBrush;
}
BOOL QnBaseDialog::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}



