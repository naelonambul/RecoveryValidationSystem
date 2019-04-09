#pragma once
#include "FileDlg.h"


// CFileView view

class CRecoveryFileView : public CView
{
	DECLARE_DYNCREATE(CRecoveryFileView)

protected:
	CRecoveryFileView();           // protected constructor used by dynamic creation
	virtual ~CRecoveryFileView();

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
	CFileDlg m_wndRecovery;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


