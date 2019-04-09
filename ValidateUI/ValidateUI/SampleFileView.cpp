// SampleFileView.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "SampleFileView.h"


// CSampleFileView

IMPLEMENT_DYNCREATE(CSampleFileView, CView)

CSampleFileView::CSampleFileView()
{
	m_wndSample.m_pParentView = this;
}

CSampleFileView::~CSampleFileView()
{
}

BEGIN_MESSAGE_MAP(CSampleFileView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSampleFileView drawing

void CSampleFileView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CSampleFileView diagnostics

#ifdef _DEBUG
void CSampleFileView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSampleFileView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSampleFileView message handlers


int CSampleFileView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_wndSample.Create(IDD_FILE_FORM, this);
	m_wndSample.PictureChange(IDB_BITMAP_SAMPLE);
	m_wndSample.ShowWindow(SW_SHOW);
	return 0;
}


void CSampleFileView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	m_wndSample.MoveWindow(0, 0, cx, cy);
}
