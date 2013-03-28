
// TinnySpriteView.cpp : implementation of the CTinnySpriteView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "TinnySprite.h"
#endif

#include "TinnySpriteDoc.h"
#include "TinnySpriteView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTinnySpriteView

IMPLEMENT_DYNCREATE(CTinnySpriteView, CView)

BEGIN_MESSAGE_MAP(CTinnySpriteView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTinnySpriteView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTinnySpriteView construction/destruction

CTinnySpriteView::CTinnySpriteView()
{
	// TODO: add construction code here

}

CTinnySpriteView::~CTinnySpriteView()
{
}

BOOL CTinnySpriteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CTinnySpriteView drawing

void CTinnySpriteView::OnDraw(CDC* /*pDC*/)
{
	CTinnySpriteDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CTinnySpriteView printing


void CTinnySpriteView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CTinnySpriteView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTinnySpriteView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTinnySpriteView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTinnySpriteView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTinnySpriteView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTinnySpriteView diagnostics

#ifdef _DEBUG
void CTinnySpriteView::AssertValid() const
{
	CView::AssertValid();
}

void CTinnySpriteView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTinnySpriteDoc* CTinnySpriteView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTinnySpriteDoc)));
	return (CTinnySpriteDoc*)m_pDocument;
}
#endif //_DEBUG


// CTinnySpriteView message handlers
