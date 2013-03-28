#pragma once

#include <afxwin.h>
#include <Gdiplus.h>
using namespace Gdiplus;

class ThemeDialog{
public:
	ThemeDialog();

	// 图片背景
	CBitmap* m_pBmpActiveBorder;
	CBitmap* m_pBmpInActiveBorder;
	CBitmap* m_pBmpBackground;

	// 最小化与最大化按钮
	Image* m_pImageMaxBtn;
	Image* m_pImageRestoreBtn;
	Image* m_pImageMinBtn;
	Image* m_pImageCloseBtn;

	// 位置
	UINT m_nTopLeft;
	UINT m_nTopRight;
	UINT m_nBottomLeft;
	UINT m_nBottomRight;

	UINT m_nToLeft;
	UINT m_nToRight;
	UINT m_nToTop;
	UINT m_nToBottom;

	UINT m_nAlignMax;
	UINT m_nAlignMin;
	UINT m_nAlignClose;

	POINT m_ptMax;
	POINT m_ptMin;
	POINT m_ptClose;

	void SetBorder(CBitmap* activeBmp, CBitmap* inactiveBmp);
	void SetMargins(UINT nTopLeft, UINT nTopRight, UINT nBottomLeft, UINT nBottomRight, UINT nToLeft, UINT nToRight, UINT nToTop, UINT nToBottom);
	void SetMaxButton(UINT nAlign, POINT point, Image *pMaxImage, Image *pRestoreImage);
	void SetMinButton(UINT nAlign, POINT point, Image *pImage);
	void SetCloseButton(UINT nAlign, POINT point, Image* pImage);
	void SetBackground(CBitmap* bmp);
};

CBitmap* LoadBmpImage(LPCTSTR szPath);
Image* LoadPngImage(LPCTSTR szPath);
void DrawPngImage(CDC* pDC, Image* pImage, int dcx, int dcy, CRect rtImage);