
// BaikuDemoView.h : interface of the CBaikuDemoView class
//

#pragma once


class CBaikuDemoView : public CHtmlView
{
protected: // create from serialization only
	CBaikuDemoView();
	DECLARE_DYNCREATE(CBaikuDemoView)

// Attributes
public:
	CBaikuDemoDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CBaikuDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BaikuDemoView.cpp
inline CBaikuDemoDoc* CBaikuDemoView::GetDocument() const
   { return reinterpret_cast<CBaikuDemoDoc*>(m_pDocument); }
#endif

