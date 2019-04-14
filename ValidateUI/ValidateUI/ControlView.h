#pragma once
#include "ControlDlg.h"


// CControlView view

class CControlView : public CView
{
	DECLARE_DYNCREATE(CControlView)

protected:
	CControlView();           // protected constructor used by dynamic creation
	virtual ~CControlView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CControlDlg m_wndControl;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PostNcDestroy();
};


