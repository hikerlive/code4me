#pragma once

#include "QnDC.h"

class QnBaseDialog : public CDialogEx {
	DECLARE_DYNAMIC(QnBaseDialog)
public:
	QnBaseDialog(UINT uiid, CWnd* parent=0);
	virtual ~QnBaseDialog();

	void CreateQnDc(UINT nBmpResId, bool bStretch=false /* «∑Ò¿≠…ÏÃÓ≥‰*/);
	void CreateQnDC(CBitmap* pBmp, bool bStretching);

protected:
	CBrush m_bgBrush;
	QnDC* m_pBgDc;

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtrlColor);
	DECLARE_MESSAGE_MAP();
};