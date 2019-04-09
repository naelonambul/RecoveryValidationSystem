// ControlView.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "ControlView.h"


// CControlView

IMPLEMENT_DYNCREATE(CControlView, CView)

CControlView::CControlView()
{

}

CControlView::~CControlView()
{
}

BEGIN_MESSAGE_MAP(CControlView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CControlView drawing

void CControlView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CControlView diagnostics

#ifdef _DEBUG
void CControlView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CControlView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CControlView message handlers


int CControlView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	m_wndControl.Create(IDD_CONTROLDLG, this);
	m_wndControl.ShowWindow(SW_SHOW);

	return 0;
}


void CControlView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_wndControl.MoveWindow(0, 0, cx, cy);
}
