// FileView.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "RecoveryFileView.h"


// CFileView

IMPLEMENT_DYNCREATE(CRecoveryFileView, CView)

CRecoveryFileView::CRecoveryFileView()
{
	m_wndRecovery.m_pParentView = this;
}

CRecoveryFileView::~CRecoveryFileView()
{
}

BEGIN_MESSAGE_MAP(CRecoveryFileView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CFileView drawing

void CRecoveryFileView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CFileView diagnostics

#ifdef _DEBUG
void CRecoveryFileView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CRecoveryFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileView message handlers


int CRecoveryFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	m_wndRecovery.Create(IDD_FILE_FORM, this);
	m_wndRecovery.PictureChange(IDB_BITMAP_RECOVERY);
	m_wndRecovery.ShowWindow(SW_SHOW);

	return 0;
}


void CRecoveryFileView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_wndRecovery.MoveWindow(0, 0, cx, cy);
}
