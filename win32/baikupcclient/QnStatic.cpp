#include "stdafx.h"
#include "QnStatic.h"

#include "atlconv.h"    // for Unicode conversion - requires #include <afxdisp.h> // MFC OLE automation classes

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TOOLTIP_ID 1

/////////////////////////////////////////////////////////////////////////////
// CQnStatic

CQnStatic::CQnStatic(COLORREF crStdColour, 
				COLORREF crHoverColour, 
				UnderLineOptions nUnderline, 
				bool bBold,
				int nFontSize,
				bool bActived)
{
    m_hLinkCursor       = NULL;                 // No cursor as yet
    m_crStdColour      = crStdColour;   // Blue
    m_crHoverColour     = crHoverColour;   // Red
    m_bOverControl      = FALSE;                // Cursor not yet over control
    m_nUnderline        = nUnderline;              // Underline the link?
	m_bBold = bBold;
	m_bActived = bActived;
	m_pToolTip = NULL;
	m_nFontSize = nFontSize;
	if (m_nFontSize <= 0)
		m_nFontSize = 12;
}

CQnStatic::~CQnStatic()
{
    m_UnderlineFont.DeleteObject();
	if (m_pToolTip)
	{
		m_pToolTip->DestroyWindow();
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
}

void CQnStatic::SetText(CString text, COLORREF crStdColour, COLORREF crHoverColour)
{
	SetWindowText(text);
	m_crStdColour      = crStdColour;   // Blue
	m_crHoverColour     = crHoverColour;   // Red
	Invalidate();
}

CString CQnStatic::GetText()
{
	return _T("");
}
/////////////////////////////////////////////////////////////////////////////
// CQnStatic overrides

BOOL CQnStatic::DestroyWindow() 
{
	return CStatic::DestroyWindow();
}

BOOL CQnStatic::PreTranslateMessage(MSG* pMsg) 
{
	if (m_pToolTip)
		m_pToolTip->RelayEvent(pMsg);
    return CStatic::PreTranslateMessage(pMsg);
}

void CQnStatic::SetToolTip(CString text)
{
	m_strToolTip = text;
}

void CQnStatic::PreSubclassWindow() 
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
   
	CFont* pFont = GetFont();
	if (!pFont)
	{
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		if (hFont == NULL)
			hFont = (HFONT) GetStockObject(ANSI_VAR_FONT);
		if (hFont)
			pFont = CFont::FromHandle(hFont);
	}
	ASSERT(pFont->GetSafeHandle());

    // Create the underline font
    LOGFONT lf;
    pFont->GetLogFont(&lf);
	lf.lfHeight = m_nFontSize;
	lf.lfWeight = m_bBold?FW_BOLD:FW_NORMAL;
	m_StdFont.CreateFontIndirect(&lf);
    SetFont(&m_StdFont);

	lf.lfUnderline = (BYTE) TRUE;
    m_UnderlineFont.CreateFontIndirect(&lf);

    SetDefaultCursor();      // Try and load up a "hand" cursor

    CStatic::PreSubclassWindow();
}

BEGIN_MESSAGE_MAP(CQnStatic, CStatic)
    //{{AFX_MSG_MAP(CQnStatic)
    ON_WM_CTLCOLOR_REFLECT()
    ON_WM_SETCURSOR()
    ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQnStatic message handlers

HBRUSH CQnStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    ASSERT(nCtlColor == CTLCOLOR_STATIC);

    if (m_bOverControl)
        pDC->SetTextColor(m_crHoverColour);
    else
        pDC->SetTextColor(m_crStdColour);

    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH)GetStockObject(NULL_BRUSH);
}

void CQnStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bActived && !m_bOverControl)        // Cursor has just moved over control
	{
		m_bOverControl = TRUE;
		if (!m_strToolTip.IsEmpty())
		{
			CRect rect; 
			GetClientRect(rect);
			if (NULL == m_pToolTip)
			{
				m_pToolTip = new CToolTipCtrl();
				m_pToolTip->Create(this);
			}
			m_pToolTip->AddTool(this, m_strToolTip, rect, TOOLTIP_ID);
		}

		TRACKMOUSEEVENT tme;         
		tme.cbSize = sizeof(TRACKMOUSEEVENT);         
		tme.dwFlags = TME_LEAVE; //监控鼠标离开         
		tme.hwndTrack = this->m_hWnd;         
		if(::_TrackMouseEvent(&tme))         
		{         
			if (m_nUnderline == ulHover)
			{
				SetFont(&m_UnderlineFont);	
				Invalidate();
			}
		}   	
	}
    CStatic::OnMouseMove(nFlags, point);
}

void CQnStatic::OnMouseLeave()      
{         
	if (m_bActived && m_bOverControl)        // Cursor has just moved over control
	{
		m_bOverControl = FALSE;
		if (m_nUnderline == ulHover)
			SetFont(&m_StdFont);	
		Invalidate();
	}       
}     


BOOL CQnStatic::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
    if (m_bActived && m_hLinkCursor)
    {
        ::SetCursor(m_hLinkCursor);
        return TRUE;
    }
    return FALSE;
}

void CQnStatic::SetDefaultCursor()
{
    if (m_hLinkCursor == NULL)                // No cursor handle - load our own
    {
        // Get the windows directory
        CString strWndDir;
        GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);
        strWndDir.ReleaseBuffer();

        strWndDir += _T("\\winhlp32.exe");
        // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
        HMODULE hModule = LoadLibrary(strWndDir);
        if (hModule) {
            HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
            if (hHandCursor)
                m_hLinkCursor = CopyCursor(hHandCursor);
        }
        FreeLibrary(hModule);
    }
}


BOOL CQnStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(rect, &CBrush(RGB(233,233,233)));
	return CStatic::OnEraseBkgnd(pDC);
}
