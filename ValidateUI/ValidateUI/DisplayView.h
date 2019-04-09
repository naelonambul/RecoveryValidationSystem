#pragma once
#include "MachineDlg.h"


// CDisplayView view

class CDisplayView : public CView
{
	DECLARE_DYNCREATE(CDisplayView)
public:
	CMachineDlg m_wndMachine07;
	CMachineDlg m_wndMachine08;
	CMachineDlg m_wndMachine10;

	CDC memdc;
	CDC*mypDc = nullptr;

	CBitmap *pOldBitmap = nullptr;
	CBitmap bitmap;
protected:
	CDisplayView();           // protected constructor used by dynamic creation
	virtual ~CDisplayView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CMachineDlg& getMachine07();
	CMachineDlg& getMachine08();
	CMachineDlg& getMachine10();
	afx_msg void OnPaint();
};


