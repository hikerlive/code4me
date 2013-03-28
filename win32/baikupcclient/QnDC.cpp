#include "stdafx.h"
#include "QnDC.h"

QnDC::QnDC()
{

}

QnDC::~QnDC()
{

}

void QnDC::CreateFromDC(CDC* pDC, CWnd* pWnd)
{
	m_pWnd = pWnd;
	m_pWnd->GetClientRect(m_dcRect);
	CreateCompatibleDC(pDC);
	m_bitmap.CreateDiscardableBitmap(pDC, m_dcRect.Width(), m_dcRect.Height());
	m_pOldBitmap = SelectObject(&m_bitmap);
	SetWindowOrg(m_dcRect.left, m_dcRect.top);
}

void QnDC::DrawToDC(CDC* pDC, CWnd* pWnd)
{
	CRect rtSrc;
	pWnd->GetWindowRect(rtSrc);
	m_pWnd->ScreenToClient(rtSrc);

	CRect rtDest;
	m_pWnd->GetWindowRect(rtDest);
	pWnd->ScreenToClient(rtDest);

	if (rtSrc.left < 0)	rtSrc.left = 0;
	if (rtSrc.top < 0) 	rtSrc.top = 0;
	if (rtDest.left < 0) rtDest.left = 0;
	if (rtDest.top < 0) rtDest.top = 0;
	::BitBlt(pDC->m_hDC, rtDest.left, rtDest.top, rtSrc.Width(), rtSrc.Height(), 
		m_hDC, rtSrc.left, rtSrc.top, SRCCOPY);
}

void QnDC::DrawStretchBitmap(CBitmap* pBmp)
{
	BITMAP bmp;
	pBmp->GetBitmap(&bmp);
	HDC memdc = ::CreateCompatibleDC(NULL);
	HBITMAP h = (HBITMAP)::SelectObject(memdc, pBmp->GetSafeHandle());
	::StretchBlt(m_hDC, m_dcRect.left, m_dcRect.top, m_dcRect.Width(), 
		m_dcRect.Height(), memdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
	::SelectObject(memdc, h);	
	::DeleteDC(memdc);
}

void QnDC::DrawFillBitmap(CBitmap* pBmp)
{
	CBrush br(pBmp);
	FillRect(m_dcRect, &br);
}

// 获取当前窗体对应区域的图片
HBITMAP QnDC::GetBitmap(CWnd* pWnd)
{
	CRect rect;
	pWnd->GetWindowRect(rect);
	m_pWnd->ScreenToClient(rect);
	return GetBitmapFromDC(rect);
}

// 获取区域内的图片
HBITMAP QnDC::GetBitmapFromDC(CRect rect)
{
	CDC mdc;
	HBITMAP hMemBmp = ::CreateCompatibleBitmap(m_hDC, rect.Width(), rect.Height());  
	mdc.CreateCompatibleDC(this);
	HBITMAP  hOldBmp = (HBITMAP)::SelectObject(mdc.m_hDC, hMemBmp);   
	::StretchBlt(mdc.m_hDC, 0, 0, rect.Width(), rect.Height(), m_hDC, 
					rect.left, rect.top, rect.Width(), rect.Height(), SRCCOPY);   
	hMemBmp = (HBITMAP)::SelectObject(mdc.m_hDC, hOldBmp);   
	::DeleteObject(hOldBmp);   
	return   hMemBmp; 
}