#pragma once

//Arrow drawing is ported from the FLATGUI version in Delphi.
//You are free to use and modify this as long as you don`t claim it .
//Copyright : Tomkat(.ro) 2004

class CQnToolTipCtrl : public CToolTipCtrl
{
	DECLARE_DYNAMIC(CQnToolTipCtrl)
	enum	Orientations
	{
		NW=1,
		NE,
		SW,
		SE,
	};
public:
	CQnToolTipCtrl();
	virtual ~CQnToolTipCtrl();
protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
private:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
public:
	COLORREF	m_bkColor;//=RGB(255,255,255);
	COLORREF	m_leftColor;//=RGB(255, 210, 83);
	COLORREF	m_frameColor;//=RGB(155, 110, 53);
	COLORREF	m_textColor;//=RGB(0,0,0);
	COLORREF	m_arrowColor;//=RGB(0,0,0);
};