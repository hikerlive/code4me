#pragma once

class QnButton : public CButton 
{
	DECLARE_DYNAMIC(QnButton)
public:
	QnButton();
	virtual ~QnButton();
	void SetImage(Gdiplus::Image* pImage, CRect rtNormalImage, CRect rtFocusImage);

protected:
	Gdiplus::Image* m_pImage;
	CRect m_rtNormalImage;
	CRect m_rtFocusImage;
	bool m_bFocus;

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItem);
	afx_msg void OnMouseMove(UINT uFlags, CPoint point);
	afx_msg void OnMouseLeave();
	DECLARE_MESSAGE_MAP()
};