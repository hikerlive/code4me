#include "stdafx.h"
#include "Theme.h"

ThemeDialog::ThemeDialog() 
{
	m_pBmpActiveBorder = 0;
	m_pBmpInActiveBorder = 0;
	m_pBmpBackground = 0;

	m_pImageMaxBtn = 0;
	m_pImageRestoreBtn = 0;
	m_pImageMinBtn = 0;
	m_pImageCloseBtn = 0;

	m_nTopLeft = 0;
	m_nTopRight = 0;
	m_nBottomLeft = 0;
	m_nBottomRight = 0;

	m_nToLeft = 0;
	m_nToRight = 0;
	m_nToTop = 0;
	m_nToBottom = 0;
}

void ThemeDialog::SetBorder(CBitmap* activeBmp, CBitmap* inactiveBmp)
{
	m_pBmpActiveBorder = activeBmp;
	m_pBmpInActiveBorder = inactiveBmp;
}

void ThemeDialog::SetMargins(UINT nTopLeft, UINT nTopRight, UINT nBottomLeft, UINT nBottomRight, UINT nToLeft, UINT nToRight, UINT nToTop, UINT nToBottom)
{
	m_nTopLeft = nTopLeft;
	m_nTopRight = nTopRight;
	m_nBottomLeft = nBottomLeft;
	m_nBottomRight = nBottomRight;
	m_nToLeft = nToLeft;
	m_nToRight = nToRight;
	m_nToTop = nToTop;
	m_nToBottom = nToBottom;
}

void ThemeDialog::SetMaxButton(UINT nAlign, POINT point, Image *pMaxImage, Image *pRestoreImage)
{
	m_nAlignMax = nAlign;
	m_ptMax = point;
	m_pImageMaxBtn = pMaxImage;
	m_pImageRestoreBtn = pRestoreImage;
}

void ThemeDialog::SetMinButton(UINT nAlign, POINT point, Image *pImage)
{
	m_nAlignMin = nAlign;
	m_ptMin = point;
	m_pImageMinBtn = pImage;
}

void ThemeDialog::SetCloseButton(UINT nAlign, POINT point, Image* pImage)
{
	m_nAlignClose = nAlign;
	m_ptClose = point;
	m_pImageCloseBtn = pImage;
}
void ThemeDialog::SetBackground(CBitmap* bmp)
{
	m_pBmpBackground = bmp;
}
CBitmap* LoadBmpImage(LPCTSTR szPath) 
{
	if (szPath) {
		HANDLE handle = ::LoadImage(NULL, szPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
		if (handle) {
			CBitmap* pBitmap = new CBitmap();
			if (!pBitmap->Attach(handle)) {
				DeleteObject(handle);
				return NULL;
			}
			return pBitmap;
		}
	}
	return NULL;
}
Image* LoadPngImage(LPCTSTR szPath)
{
	USES_CONVERSION;
	return Image::FromFile(szPath);
}
void DrawPngImage(CDC* pDC, Image* pImage, int dcx, int dcy, CRect rtImage)
{
	Gdiplus::Graphics graph(pDC->m_hDC);
	graph.DrawImage(
		pImage, 
		Gdiplus::RectF(0, 0, rtImage.Width(), rtImage.Height()),
		rtImage.left, rtImage.top,
		rtImage.Width(), rtImage.Height(), 
		Gdiplus::UnitPixel
	);
	graph.ReleaseHDC(pDC->m_hDC);
}