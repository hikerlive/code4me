#if !defined(AFX_QNSTATIC_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_QNSTATIC_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
#include "QnToolTipCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CQnStatic window

class CQnStatic : public CStatic
{
// Construction/destruction
public:
	enum UnderLineOptions { ulHover = -1, ulNone = 0, ulAlways = 1};
public:
    CQnStatic(COLORREF crStdColour = RGB(22,112,235), 
			COLORREF crHoverColour = RGB(22,112,235), 
			UnderLineOptions nUnderline = ulHover,
			bool bBold = false,
			int nFontSize = 12,
			bool bActived = true);

    virtual ~CQnStatic();

// Attributes
public:
	void SetToolTip(CString text);
	void SetText(CString text, COLORREF crStdColour, COLORREF crHoverColour);
	CString GetText();
	void SetActived(bool bEnable) { m_bActived = bEnable; }
	bool IsActived() { return m_bActived; }
	//void SetBold(bool bEnable) { m_bBold = bEnable ; }
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CQnStatic)
	public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
    void SetDefaultCursor();

// Protected attributes
protected:
	UnderLineOptions	m_nUnderline;
	int		m_nFontSize;
	bool	m_bActived;
	COLORREF m_crStdColour; 
    COLORREF m_crHoverColour;                       // Hover colour
    BOOL     m_bOverControl;                        // cursor over control?
	BOOL	 m_bBold;
    CFont    m_UnderlineFont;                       // Font for underline display
    CFont    m_StdFont;                             // Standard font
    HCURSOR  m_hLinkCursor;                         // Cursor for hyperlink
    CToolTipCtrl* m_pToolTip;                         // The tooltip
	CString m_strToolTip;
    // Generated message map functions
protected:
    //{{AFX_MSG(CQnStatic)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave() ;
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QNSTATIC_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
