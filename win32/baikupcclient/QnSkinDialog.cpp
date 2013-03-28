#include "stdafx.h"
#include "QnSkinDialog.h"

#pragma warning(disable:4244)

IMPLEMENT_DYNAMIC(QnSkinDialog, QnBaseDialog)

QnSkinDialog::QnSkinDialog(UINT nIDTemplate, CWnd* parent) :
	QnBaseDialog(nIDTemplate, parent),
	m_pThemeDialog(0),
	m_nMoveHitTest(0),
	m_nDownHitTest(0),
	m_pIconImage(0),
	m_rtIconImage(0, 0, 0, 0),
	m_bActived(FALSE),
	m_bFullScreen(FALSE),
	m_ptTitle(-1, -1),
	m_bTitleBold(false),
	m_clTitle(RGB(0,0,0)),
	m_bCancelReSizeStyle(false),
	m_nlimitedWidth(0),
	m_nlimitedHeight(0)
{
	m_strTitle = _T("佰酷");
}

QnSkinDialog::~QnSkinDialog()
{

}

BEGIN_MESSAGE_MAP(QnSkinDialog, QnBaseDialog)
END_MESSAGE_MAP()

LRESULT QnSkinDialog::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pThemeDialog) {
		switch(message) {
		case WM_ACTIVATE:
			OnThemeActivate(wParam, CWnd::FromHandle((HWND)lParam), 0);
			return 0;
		case WM_NCACTIVATE:
			return OnThemeNcActivate((BOOL)wParam);
		case WM_NCCALCSIZE:
			OnThemeNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS *)lParam);
			break;
		case WM_NCHITTEST: {
				UINT nHitTest = OnThemeNcHitTest(CPoint(lParam));
				if (HTCLIENT != nHitTest) {
					return nHitTest;
				}
			}break;
		case WM_NCPAINT:
			OnThemeNcPaint();
			return 0;
		case WM_NCLBUTTONDOWN: {
			if (OnThemeNcLButtonDown(wParam, CPoint(lParam)))
				return 0;
			}break;
		case WM_NCLBUTTONUP: {
			if (OnThemeNcLButtonUp(wParam, CPoint(lParam)))
				return 0;
			}break;
		case WM_NCLBUTTONDBLCLK: {
			OnThemeNcLButtonDblClk(wParam, CPoint(lParam));
			}break;
		case WM_NCRBUTTONDOWN:
			OnThemeNcRButtonDown(wParam, CPoint(lParam));
			return 0;
		case WM_NCRBUTTONUP:
			OnThemeNcRButtonUp(wParam,CPoint(lParam));
			return 0;
		case WM_LBUTTONUP:
			OnThemeLButtonUp(UINT(wParam), CPoint(lParam));
			break;
		case WM_NCMOUSEMOVE:
			OnThemeNcMouseMove(wParam, CPoint(lParam));
			break;
		case WM_GETMINMAXINFO:
			OnThemeGetMinMaxInfo((MINMAXINFO*)lParam);
			break;
		case WM_CREATE:
			OnThemeResize();
			break;
		case WM_NCCREATE:
			DisableAeroStyle(m_hWnd);
			break;
		default:
			break;
		}
	}
	return QnBaseDialog::WindowProc(message, wParam, lParam);
}

void QnSkinDialog::OnThemeNcPaint()
{
	CDC* pDc = GetWindowDC();
	CDC memDc;
	memDc.CreateCompatibleDC(pDc);

	CBitmap* bitmap = m_pThemeDialog->m_pBmpActiveBorder;
	if (bitmap == 0)
		return;

	BITMAP bm;
	bitmap->GetBitmap(&bm);

	CRect rcBmp;
	rcBmp.left = 0;
	rcBmp.right = bm.bmWidth;
	rcBmp.top = 0;
	rcBmp.bottom = bm.bmHeight;

	CRect rcWin;
	GetWindowRect(&rcWin);
	rcWin -= CPoint(rcWin.left, rcWin.top);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDc, rcWin.Width(),m_pThemeDialog->m_nToTop);
	
	CBitmap* pOldBmp = memDc.SelectObject(&bmp);
	DrawBorder(pDc, rcWin, rcBmp, TRUE);
	rcWin.bottom = rcWin.top + m_pThemeDialog->m_nToTop;
	DrawCaption(&memDc, rcWin, rcBmp, TRUE);
	pDc->BitBlt(0, 0, rcWin.Width(), rcWin.Height(), &memDc, 0, 0, SRCCOPY);
	memDc.SelectObject(pOldBmp);

	memDc.DeleteDC();
	ReleaseDC(pDc);
}
void QnSkinDialog::DrawBorder(CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bActive)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* bitmap = memDC.SelectObject(bActive?m_pThemeDialog->m_pBmpActiveBorder : m_pThemeDialog->m_pBmpInActiveBorder);
	if (bitmap == 0)
		return;

	// left
	CRect rc1(0, m_pThemeDialog->m_nToTop, m_pThemeDialog->m_nToLeft, rcDest.Height()-m_pThemeDialog->m_nToBottom);
	CRect rc2(0, m_pThemeDialog->m_nToTop, m_pThemeDialog->m_nToLeft, rcSrc.Height()-m_pThemeDialog->m_nToBottom);
	StretchDraw(pDC, &memDC, rc1, rc2);


	//right
	rc1 = CRect(rcDest.right-m_pThemeDialog->m_nToRight, m_pThemeDialog->m_nToTop, rcDest.right, rcDest.bottom-m_pThemeDialog->m_nToBottom);
	rc2 = CRect(rcSrc.right-m_pThemeDialog->m_nToRight, m_pThemeDialog->m_nToTop, rcSrc.right, rcSrc.bottom-m_pThemeDialog->m_nToBottom);
	StretchDraw(pDC, &memDC, rc1, rc2); 
	//bottom
	rc1 = CRect(m_pThemeDialog->m_nBottomLeft, rcDest.Height()-m_pThemeDialog->m_nToBottom, rcDest.Width()-m_pThemeDialog->m_nBottomRight, rcDest.Height());
	rc2 = CRect(m_pThemeDialog->m_nBottomLeft, rcSrc.Height()-m_pThemeDialog->m_nToBottom, rcSrc.Width()-m_pThemeDialog->m_nBottomRight, rcSrc.Height());
	StretchDraw(pDC, &memDC, rc1, rc2); 
	//bottomLeft
	pDC->BitBlt(0, rcDest.Height()-m_pThemeDialog->m_nToBottom, m_pThemeDialog->m_nBottomLeft, m_pThemeDialog->m_nToBottom, 
		&memDC, 0, rcSrc.Height()-m_pThemeDialog->m_nToBottom, SRCCOPY);
	//bottomRight
	pDC->BitBlt(rcDest.right-m_pThemeDialog->m_nBottomRight, rcDest.Height()-m_pThemeDialog->m_nToBottom, m_pThemeDialog->m_nBottomRight, m_pThemeDialog->m_nToBottom, 
		&memDC, rcSrc.right-m_pThemeDialog->m_nBottomRight, rcSrc.Height()-m_pThemeDialog->m_nToBottom, SRCCOPY);
	
	memDC.SelectObject(bitmap);
	memDC.DeleteDC();
}
void QnSkinDialog::DrawCaption(CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bActive)
{
	// 画背景
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = memDC.SelectObject(bActive ? m_pThemeDialog->m_pBmpActiveBorder : m_pThemeDialog->m_pBmpInActiveBorder);
	if (pOldBitmap == NULL)
		return;

	//topLeft
	pDC->BitBlt(0, 0, m_pThemeDialog->m_nTopLeft, m_pThemeDialog->m_nToTop, 
		&memDC, 0, 0, SRCCOPY);
	//top
	CRect rc1(m_pThemeDialog->m_nTopLeft, 0, rcDest.Width()-m_pThemeDialog->m_nTopRight, m_pThemeDialog->m_nToTop);
	CRect rc2(m_pThemeDialog->m_nTopLeft, 0, rcSrc.Width()-m_pThemeDialog->m_nTopRight, m_pThemeDialog->m_nToTop);
	StretchDraw(pDC, &memDC, rc1, rc2);

	//topRight
	pDC->BitBlt(rcDest.right-m_pThemeDialog->m_nTopRight, 0, m_pThemeDialog->m_nTopRight, m_pThemeDialog->m_nToTop, 
		&memDC, rcSrc.right-m_pThemeDialog->m_nTopRight, 0, SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	// 画其他
	Graphics graph(pDC->m_hDC);
	
	// 画图标
	if (m_pIconImage) {
		if (m_rtIconImage.Width() <= 0) {
			m_rtIconImage.left = 0;
			m_rtIconImage.right = m_pIconImage->GetWidth();
		}
		if (m_rtIconImage.Height() <= 0) {
			m_rtIconImage.top = 0;
			m_rtIconImage.bottom = m_pIconImage->GetHeight();
		}
		int dcy = m_rtIconImage.Height()>=24?4:(24-m_rtIconImage.Height())/2+4;
		graph.DrawImage(m_pIconImage, RectF(m_pThemeDialog->m_nToLeft, dcy, m_rtIconImage.Width(), m_rtIconImage.Height()), 
			m_rtIconImage.left, m_rtIconImage.top, m_rtIconImage.Width(), m_rtIconImage.Height(), UnitPixel);
	}

	// 写标题
	if (_tcslen(m_strTitle)) {
// 		CString str;
// 		GetWindowText( str );
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfWeight = m_bTitleBold?FW_BOLD:FW_NORMAL;
		lf.lfHeight = 12;
		CFont boldFont;
		boldFont.CreateFontIndirect(&lf);
		CFont *pOldFont = pDC->SelectObject(&boldFont);
 		pDC->SetTextColor(m_clTitle);

		// RGB(158,11,11)
		// RGB(0,70,213)
		pDC->SetBkMode(TRANSPARENT);
		CRect rText = rcDest;
		rText.right -= m_pThemeDialog->m_nToRight;		
		if (m_ptTitle.x < 0 || m_ptTitle.y < 0) {
			rText.top += 4;
			rText.left += m_pThemeDialog->m_nToLeft+m_rtIconImage.Width()+2;
		} else {
			rText.top = m_ptTitle.y;
			rText.left = m_ptTitle.x;
		}
		pDC->DrawText( m_strTitle, rText, DT_SINGLELINE | DT_LEFT | DT_VCENTER |DT_END_ELLIPSIS);
		pDC->SelectObject(pOldFont);
	}

	//画最小与最大与关闭按钮
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & WS_SYSMENU) {
		UINT nHitTest = 0;
		UINT nButtonState = 0;
		if (m_nDownHitTest) {
			if (m_nDownHitTest == m_nMoveHitTest) {
				nHitTest = m_nDownHitTest;
				nButtonState = 1;
			}
		}
		else if (m_nMoveHitTest) {
			nHitTest = m_nMoveHitTest;
			nButtonState = 2;
		}

		Image* pObjImage;
		CRect rc;

		GetCaptionButtonRect(rc, SC_CLOSE);
		pObjImage = m_pThemeDialog->m_pImageCloseBtn;
		if (nHitTest == HTCLOSE) {
			graph.DrawImage(pObjImage, RectF(rc.left, rc.top, rc.Width(), rc.Height()), 
				rc.Width()*nButtonState, 0, rc.Width(), rc.Height(), UnitPixel);
		} else {
			graph.DrawImage(pObjImage, RectF(rc.left, rc.top, rc.Width(), rc.Height()), 
				0, 0, rc.Width(), rc.Height(), UnitPixel);
		}	
		if ( dwStyle & WS_MAXIMIZEBOX ) {	
			if (IsZoomed())
				pObjImage = m_pThemeDialog->m_pImageRestoreBtn;
			else
				pObjImage = m_pThemeDialog->m_pImageMaxBtn;
				
			GetCaptionButtonRect(rc, SC_MAXIMIZE);

			if (nHitTest == HTMAXBUTTON)
				graph.DrawImage(pObjImage, RectF(rc.left, rc.top, rc.Width(), rc.Height()), 
					rc.Width()*nButtonState, 0, rc.Width(), rc.Height(), UnitPixel);
			else
				graph.DrawImage(pObjImage, RectF(rc.left, rc.top, rc.Width(), rc.Height()), 
					0, 0, rc.Width(), rc.Height(), UnitPixel);
		}
		if ( dwStyle & WS_MINIMIZEBOX ) {
			pObjImage = m_pThemeDialog->m_pImageMinBtn;
			GetCaptionButtonRect(rc, SC_MINIMIZE);
			if (nHitTest == HTMINBUTTON)
				graph.DrawImage(pObjImage, RectF(rc.left, rc.top, rc.Width(), rc.Height()), 
					rc.Width()*nButtonState, 0, rc.Width(), rc.Height(), UnitPixel);
			else
				graph.DrawImage(pObjImage, RectF(rc.left, rc.top, rc.Width(), rc.Height()), 
					0, 0, rc.Width(), rc.Height(), UnitPixel);
		}
	}
	graph.ReleaseHDC(pDC->m_hDC);
}
void QnSkinDialog::StretchDraw(CDC* pDCDest, CDC* pDCSrc, CRect rcDest, CRect rcSrc)
{
	int nWidth = (rcDest.Width() > rcSrc.Width()) ? rcSrc.Width() : rcDest.Width();
	int nHeight = (rcDest.Height() > rcSrc.Height()) ? rcSrc.Height() : rcDest.Height();
	if (rcDest.Height() == rcSrc.Height()) {
		while (rcDest.Width() > 0) {
			pDCDest->BitBlt( rcDest.left, rcDest.top, nWidth, nHeight, 
				pDCSrc, rcSrc.left, rcSrc.top, SRCCOPY);
			rcDest.left += rcSrc.Width();
			nWidth = (rcDest.Width() > rcSrc.Width()) ? rcSrc.Width() : rcDest.Width();
		}
	}
	else if (rcDest.Width() == rcSrc.Width()) {
		while (rcDest.Height() > 0) {
			pDCDest->BitBlt( rcDest.left, rcDest.top, nWidth, nHeight, 
				pDCSrc, rcSrc.left, rcSrc.top, SRCCOPY);
			rcDest.top += rcSrc.Height();
			nHeight = (rcDest.Height() > rcSrc.Height()) ? rcSrc.Height() : rcDest.Height();
		}
	}
}
void QnSkinDialog::GetCaptionButtonRect(CRect &rect, UINT nType)
{
	UINT nAlign;
	POINT ptOffset;
	Image* pBitmap = NULL;
	BITMAP bm;
	switch (nType)
	{
	case SC_CLOSE:
		nAlign = m_pThemeDialog->m_nAlignClose;
		pBitmap = m_pThemeDialog->m_pImageCloseBtn;
		ptOffset = m_pThemeDialog->m_ptClose;
		break;
	case SC_MAXIMIZE:
		nAlign = m_pThemeDialog->m_nAlignMax;
		pBitmap = m_pThemeDialog->m_pImageMaxBtn;
		ptOffset = m_pThemeDialog->m_ptMax;
		break;
	case SC_MINIMIZE:
		nAlign = m_pThemeDialog->m_nAlignMax;
		pBitmap = m_pThemeDialog->m_pImageMinBtn;
		if (GetWindowLong(m_hWnd, GWL_STYLE) & WS_MAXIMIZEBOX)
			ptOffset = m_pThemeDialog->m_ptMin;
		else
			ptOffset = m_pThemeDialog->m_ptMax;
		break;
	default:
		break;
	}
	if (NULL != pBitmap)
	{
		bm.bmWidth = pBitmap->GetWidth();
		bm.bmHeight = pBitmap->GetHeight();
	}
	
	CRect rcWin;
	GetWindowRect(&rcWin);

	// 右对齐
	if (nAlign)
	{
		rect.right = rcWin.Width() - ptOffset.x;
		rect.left = rect.right - bm.bmWidth/4;
	}
	// 左对齐
	else
	{
		rect.left = ptOffset.x;
		rect.right = rect.left + bm.bmWidth/4;
	}
	rect.top = ptOffset.y;
	rect.bottom = rect.top + bm.bmHeight;
}
void QnSkinDialog::SetImageIcon(Image* pIconImage, CRect rtImgSrc)
{
	m_pIconImage = pIconImage;
	m_rtIconImage = rtImgSrc;
}
void QnSkinDialog::SetTitle(const char* pszTitle, int x, int y, COLORREF clText, bool bBold)
{
	m_strTitle = pszTitle;
	m_clTitle = clText;
	m_bTitleBold = bBold;
	m_ptTitle.SetPoint(x, y);
}
void QnSkinDialog::DisableAeroStyle(HWND hWnd) 
{
	typedef HRESULT (WINAPI *DWMSETWINDOWATTRIBUTE)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
	typedef HRESULT (WINAPI *DWMGETWINDOWATTRIBUTE)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
	HMODULE hModule=LoadLibraryA("dwmapi.dll");
	if(hModule)
	{
		DWMGETWINDOWATTRIBUTE pDwmGetWindowAttributeProc=(DWMGETWINDOWATTRIBUTE)GetProcAddress(hModule,"DwmGetWindowAttribute");
		if(pDwmGetWindowAttributeProc)
		{
			BOOL enabled;
			HRESULT hr=pDwmGetWindowAttributeProc(hWnd,/*DWMWA_NCRENDERING_ENABLED*/1,&enabled,sizeof(enabled));
			if(SUCCEEDED(hr) && enabled)
			{
				DWMSETWINDOWATTRIBUTE pDwmSetWindowAttributeProc=(DWMSETWINDOWATTRIBUTE)GetProcAddress(hModule,"DwmSetWindowAttribute"); 
				if (pDwmSetWindowAttributeProc)
				{
					int ncrp = 1; //DWMNCRP_DISABLED:1;DWMNCRP_ENABLED:2
					pDwmSetWindowAttributeProc(hWnd, 2, &ncrp, sizeof(ncrp));
				}
			}
		}
		FreeLibrary(hModule);
	}
}
BOOL QnSkinDialog::PopupSysMenu(CPoint point)
{
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		if (IsZoomed()) { // maximized
			pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_ENABLED);
		} else { // restore
			pSysMenu->EnableMenuItem(SC_MINIMIZE, MF_BYCOMMAND | MF_ENABLED);
			pSysMenu->EnableMenuItem(SC_MAXIMIZE, MF_BYCOMMAND | MF_ENABLED);
			pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		}

		if (IsIconic()) { // minimized
			pSysMenu->EnableMenuItem(SC_MINIMIZE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			pSysMenu->EnableMenuItem(SC_RESTORE, MF_BYCOMMAND | MF_ENABLED);
		}
		pSysMenu->TrackPopupMenu(0, point.x, point.y, this);
	}
	return TRUE;
}
BOOL QnSkinDialog::OnThemeNcActivate(BOOL bActivate)
{
	TRACE(_T("\nQnSkinDialog::OnThemeNcActivate()"));
	// 激活窗体时归位
	if (m_nDownHitTest || m_nMoveHitTest) {
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
	}
	SetWindowLong(m_hWnd, GWL_USERDATA, bActivate);
	return !bActivate;
}
// 坐标命中测试消息
UINT QnSkinDialog::OnThemeNcHitTest(CPoint point)
{
	//TRACE(_T("QnSkinDialog::OnThemeNcHitTest()"));
	CRect rcWin;
	GetWindowRect(&rcWin);
	point = point - CPoint(rcWin.left, rcWin.top);

	CRect rect;
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	if (dwStyle & WS_SYSMENU) {
		GetCaptionButtonRect(rect, SC_CLOSE);
		if (PtInRect(rect, point)) {
			return HTCLOSE;
		}
		if (dwStyle & WS_MAXIMIZEBOX) {
			GetCaptionButtonRect(rect, SC_MAXIMIZE);
			if (PtInRect(rect, point)) {
				return HTMAXBUTTON;
			}
		}
		if (dwStyle & WS_MINIMIZEBOX) {
			GetCaptionButtonRect(rect, SC_MINIMIZE);
			if (PtInRect(rect, point)) {
				return HTMINBUTTON;
			}
		}
	}

	CRect rcCaption(&rcWin);
	rcCaption -= CPoint(rcWin.left, rcWin.top);
	rcCaption.bottom = rcCaption.top + m_pThemeDialog->m_nToTop;
	// 需要去掉边框，暂时定义为4，实际上应由边框的类型来取得。
	rcCaption.DeflateRect(4,4,4,4);
	if (PtInRect(rcCaption, point)) {
		return HTCAPTION;
	}

	return HTCLIENT;
}
// 计算客户区大小
void QnSkinDialog::OnThemeNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	TRACE(_T("QnSkinDialog::OnThemeNcCalcSize()"));
	if (bCalcValidRects) {
		int crcx = 3;
		int crcy = 3;
		int bordercx = GetSystemMetrics(SM_CXBORDER);
		int bordercy = GetSystemMetrics(SM_CYBORDER);
		int framecx = GetSystemMetrics(SM_CXFRAME);
		int framecy = GetSystemMetrics(SM_CYFRAME);
		if (GetWindowLong(m_hWnd, GWL_STYLE) & WS_SIZEBOX) {
			crcx = framecx-bordercx;
			crcy = framecy-bordercy;
		} else {
			if (m_bCancelReSizeStyle) {
				crcx = 0;
				crcy = 0;
			}
			TRACE(_T("%d,%d,%d,%d"),lpncsp->rgrc[0].left, lpncsp->rgrc[0].top, lpncsp->rgrc[0].right, lpncsp->rgrc[0].bottom);
		}

		if (m_bFullScreen) {
			TRACE(_T("full screen:%d,%d,%d,%d"),lpncsp->rgrc[0].left, lpncsp->rgrc[0].top, lpncsp->rgrc[0].right, lpncsp->rgrc[0].bottom);
			CRect rect = lpncsp->rgrc[0];
			rect.MoveToXY(-crcx, -crcy);
			lpncsp->rgrc[0] = rect;
		} else {
			int bordercx = GetSystemMetrics(SM_CXBORDER);
			int bordercy = GetSystemMetrics(SM_CYBORDER);
			int framecx = GetSystemMetrics(SM_CXFRAME);
			int framecy = GetSystemMetrics(SM_CYFRAME);

			lpncsp->rgrc[0].left += m_pThemeDialog->m_nToLeft-crcx;
			lpncsp->rgrc[0].right -= m_pThemeDialog->m_nToRight-crcx;
			lpncsp->rgrc[0].top += m_pThemeDialog->m_nToTop-crcy;
			lpncsp->rgrc[0].bottom -= m_pThemeDialog->m_nToBottom-crcy;
		}
	}
}
BOOL QnSkinDialog::OnThemeNcLButtonDown(UINT nHitTest, CPoint point)
{
	TRACE(_T("QnSkinDialog::OnThemeNcLButtonDown()"));
	if ((HTMAXBUTTON == nHitTest) || (HTMINBUTTON == nHitTest) || (HTCLOSE == nHitTest)) {
		SetCapture();
		m_nDownHitTest = nHitTest;
		m_nMoveHitTest = nHitTest;
		OnThemeNcPaint();
		return TRUE;
	} 
	else if (m_nDownHitTest != 0) {
		SetCapture();
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
		OnThemeNcPaint();
	}
	return FALSE;
}
BOOL QnSkinDialog::OnThemeNcLButtonUp(UINT nHitTest, CPoint point)
{
	TRACE(_T("QnSkinDialog::OnThemeNcLButtonUp()"));
	if (m_nDownHitTest !=0) {
		ReleaseCapture();
		if (m_nDownHitTest == nHitTest) {
			switch (m_nDownHitTest) {
			case HTMAXBUTTON: {
				if (IsZoomed()) {
					SendMessage(WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
				} else {
					SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));
				}
				return TRUE;
				}break;
			case HTMINBUTTON: {
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(point.x, point.y));
				return TRUE;
				}break;
			case HTCLOSE: {
				SendMessage(WM_SYSCOMMAND, SC_CLOSE, MAKELPARAM(point.x, point.y));
				return TRUE;
				}break;
			default:
				break;
			}
		}
	}
	return FALSE;
}
void QnSkinDialog::OnThemeNcLButtonDblClk(UINT nHitTest, CPoint point)
{
}
void QnSkinDialog::OnThemeNcRButtonDown(UINT nHitTest, CPoint point)
{
}
void QnSkinDialog::OnThemeNcRButtonUp(UINT nHitTest, CPoint point)
{
	TRACE(_T("QnSkinDialog::OnThemeNcRButtonUp"));
	// 可在这里弹出系统菜单
	if (HTCAPTION== nHitTest) {
		PopupSysMenu(point);
	}
}
void QnSkinDialog::OnThemeNcMouseMove(UINT nHitTest, CPoint point)
{
	// TRACE(_T("QnSkinDialog::OnThemeNcMouseMove()"));
	if ((HTMAXBUTTON == nHitTest) || (HTMINBUTTON == nHitTest) || (HTCLOSE == nHitTest)) {
		m_nMoveHitTest = nHitTest;
		OnThemeNcPaint();
	} 
	else if (m_nMoveHitTest != 0) {
		m_nMoveHitTest = 0;
		OnThemeNcPaint();
	}
}
void QnSkinDialog::OnThemeActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	TRACE(_T("QnSkinDialog::OnThemeActivate()"));
	if (m_nDownHitTest || m_nMoveHitTest) {
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
	}
	BOOL bActive = (nState == WA_ACTIVE || nState == WA_CLICKACTIVE);
	m_bActived = nState == WA_ACTIVE;
	NotifyActived(m_bActived);
	OnThemeNcActivate(bActive);
}
void QnSkinDialog::OnThemeLButtonUp(UINT nFlag, CPoint point)
{
	TRACE(_T("QnSkinDialog::OnThemeLButtonUp()"));
	CPoint pt = point;
	ClientToScreen(&pt);
	OnThemeNcLButtonUp(OnThemeNcHitTest(pt), point);

	if (m_nDownHitTest || m_nMoveHitTest) {
		m_nDownHitTest = 0;
		m_nMoveHitTest = 0;
		OnThemeNcPaint();
	}
}
void QnSkinDialog::OnThemeGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	TRACE(_T("QnSkinDialog::OnThemeGetMinMaxInfo()"));
}
void QnSkinDialog::OnThemeResize()
{
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyle &= ~WS_CAPTION;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	CRect rcWin;
	GetClientRect(&rcWin);
	ClientToScreen(&rcWin);

	rcWin.left -= (m_pThemeDialog->m_nToLeft);
	rcWin.right += (m_pThemeDialog->m_nToRight);
	rcWin.top -= (m_pThemeDialog->m_nToTop);
	rcWin.bottom += (m_pThemeDialog->m_nToBottom);

	::SetWindowPos(m_hWnd, 0, rcWin.left, rcWin.top, rcWin.Width(), rcWin.Height(),SWP_FRAMECHANGED);
}
