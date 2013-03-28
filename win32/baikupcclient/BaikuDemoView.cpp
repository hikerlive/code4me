
// BaikuDemoView.cpp : implementation of the CBaikuDemoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BaikuDemo.h"
#endif

#include "BaikuDemoDoc.h"
#include "BaikuDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBaikuDemoView

IMPLEMENT_DYNCREATE(CBaikuDemoView, CHtmlView)

BEGIN_MESSAGE_MAP(CBaikuDemoView, CHtmlView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CBaikuDemoView construction/destruction

CBaikuDemoView::CBaikuDemoView()
{
	// TODO: add construction code here

}

CBaikuDemoView::~CBaikuDemoView()
{
}

BOOL CBaikuDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CBaikuDemoView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();

	Navigate2(_T("http://www.msdn.microsoft.com/visualc/"),NULL,NULL);
}


// CBaikuDemoView printing


void CBaikuDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}


void CBaikuDemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CBaikuDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CBaikuDemoView diagnostics

#ifdef _DEBUG
void CBaikuDemoView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CBaikuDemoView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CBaikuDemoDoc* CBaikuDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBaikuDemoDoc)));
	return (CBaikuDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CBaikuDemoView message handlers
