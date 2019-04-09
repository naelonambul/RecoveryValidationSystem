#pragma once
#include "FileDlg.h"


// CSampleFileView view

class CSampleFileView : public CView
{
	DECLARE_DYNCREATE(CSampleFileView)

protected:
	CSampleFileView();           // protected constructor used by dynamic creation
	virtual ~CSampleFileView();

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
	CFileDlg m_wndSample;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


