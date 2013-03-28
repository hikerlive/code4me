#pragma once

#include "QnBaseDialog.h"
#include "Theme.h"

class QnSkinDialog : public QnBaseDialog {
	DECLARE_DYNAMIC(QnSkinDialog)
public:
	QnSkinDialog(UINT nIDTemplate, CWnd* parent = 0);
	virtual ~QnSkinDialog();

	void SetTheme(ThemeDialog* pTheme) { m_pThemeDialog = pTheme; }
	void SetImageIcon(Image* pIconImage, CRect rtImgSrc);
	void SetTitle(const char* title, int x=35, int y=4, COLORREF clText=RGB(0,0,0), bool bBold=true);
	virtual void NotifyActived(BOOL bActived) {}

protected:
	ThemeDialog* m_pThemeDialog;
	UINT	m_nMoveHitTest;
	UINT	m_nDownHitTest;
	Image*	m_pIconImage;
	CRect	m_rtIconImage;
	CString m_strTitle;
	CPoint	m_ptTitle;
	COLORREF m_clTitle;
	bool	m_bTitleBold;
	BOOL	m_bActived;
	BOOL	m_bFullScreen;
	bool	m_bCancelReSizeStyle;
	int		m_nlimitedWidth;
	int		m_nlimitedHeight;

protected:
	void OnThemeNcPaint();
	void OnThemeResize();
	UINT OnThemeNcHitTest(CPoint point);
	BOOL OnThemeNcActivate(BOOL bActivate);
	void OnThemeActivate(UINT nState, CWnd *pWndOther, BOOL bMinimized);
	void OnThemeLButtonUp(UINT nFlag, CPoint point);
	void OnThemeNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	BOOL OnThemeNcLButtonDown(UINT nHitTest, CPoint point);
	BOOL OnThemeNcLButtonUp(UINT nHitTest, CPoint point);
	void OnThemeNcLButtonDblClk(UINT nHitTest, CPoint point);
	void OnThemeNcRButtonDown(UINT nHitTest, CPoint point);
	void OnThemeNcRButtonUp(UINT nHitTest, CPoint point);
	void OnThemeNcMouseMove(UINT nHitTest, CPoint point);
	void OnThemeGetMinMaxInfo(MINMAXINFO FAR* lpMMI);

	void DrawBorder(CDC* pDc, CRect rcDest, CRect rcSrc, BOOL bActive);
	void DrawCaption(CDC* pDc, CRect rcDest, CRect rcSrc, BOOL bActive);
	void StretchDraw(CDC* pDCDest, CDC* pDCSrc, CRect rcDest, CRect rcSrc);
	void GetCaptionButtonRect(CRect &rect, UINT nType);
	BOOL PopupSysMenu(CPoint point);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	void DisableAeroStyle(HWND hWnd);

protected:
	DECLARE_MESSAGE_MAP()
};