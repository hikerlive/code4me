#pragma once

#include <afxwin.h>

class QnDC : public CDC {
public:
	QnDC();
	~QnDC();
	
	void DrawToDC(CDC* pDC, CWnd* pWnd);
	void DrawStretchBitmap(CBitmap* pBmp);
	void DrawFillBitmap(CBitmap* pBmp);
	void CreateFromDC(CDC* pDC, CWnd* pWnd);
	HBITMAP GetBitmap(CWnd* pWnd);
	HBITMAP GetBitmapFromDC(CRect rect);

protected:
	CWnd* m_pWnd;
	CRect m_dcRect;
	CBitmap m_bitmap;
	CBitmap* m_pOldBitmap;
};