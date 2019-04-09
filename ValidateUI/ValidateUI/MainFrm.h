
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "afxext.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CSplitterWnd m_wndSplitHor;
	CSplitterWnd m_wndSplitVer;

	CFont m_Insertfnt;
	LOGFONT m_Insertlf = { 0 };

	CFont m_FilePathfnt;
	LOGFONT m_FilePathlf = { 0 };

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	void SetInsertfnt(TCHAR* pszfntName);
	void SetFilePathfnt(TCHAR* pszfntName);
};


