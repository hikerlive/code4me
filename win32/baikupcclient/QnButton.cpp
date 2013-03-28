#include "stdafx.h"
#include "QnButton.h"

IMPLEMENT_DYNAMIC(QnButton, CButton)

QnButton::QnButton()
{
	m_bFocus = false;
}
QnButton::~QnButton()
{
}
void QnButton::SetImage(Gdiplus::Image* pImage, CRect rtNormalImage, CRect rtFocusImage)
{
	m_pImage = pImage;
	m_rtNormalImage = rtNormalImage;
	m_rtFocusImage = rtFocusImage;
}

BEGIN_MESSAGE_MAP(QnButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()

void QnButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItem) 
{
	CPoint pt;
	CRect rect;
	CDC* pDC = CDC::FromHandle(lpDrawItem->hDC);
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	GetClientRect(rect);
	
	// 鼠标在按钮上
	if (m_rtFocusImage.Width() > 0 && rect.PtInRect(pt)) {
		Gdiplus::Graphics graph(pDC->m_hDC);
		graph.DrawImage(
			m_pImage, 
			Gdiplus::RectF(0, 0, m_rtFocusImage.Width(), m_rtFocusImage.Height()),
			m_rtFocusImage.left, m_rtFocusImage.top,
			m_rtFocusImage.Width(), m_rtFocusImage.Height(), 
			Gdiplus::UnitPixel
		);
		graph.ReleaseHDC(pDC->m_hDC);
	} else {
		Gdiplus::Graphics graph(pDC->m_hDC);
		graph.DrawImage(
			m_pImage, 
			Gdiplus::RectF(0, 0, m_rtNormalImage.Width(), m_rtNormalImage.Height()),
			m_rtNormalImage.left, m_rtNormalImage.top,
			m_rtNormalImage.Width(), m_rtNormalImage.Height(), 
			Gdiplus::UnitPixel
		);
		graph.ReleaseHDC(pDC->m_hDC);
	}
}
void QnButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bFocus) {
		m_bFocus = true;
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.hwndTrack = this->m_hWnd;
		if (::_TrackMouseEvent(&tme)) {
			Invalidate();
		}
	}
}
void QnButton::OnMouseLeave()
{
	m_bFocus = false;
	Invalidate();
	CButton::OnMouseLeave();
}
