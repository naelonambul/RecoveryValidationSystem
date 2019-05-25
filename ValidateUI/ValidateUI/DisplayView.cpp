// DisplayView.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "DisplayView.h"

#define MAX_SCREEN_X 1920
#define MAX_SCREEN_Y 1080
// CDisplayView

IMPLEMENT_DYNCREATE(CDisplayView, CView)

CDisplayView::CDisplayView()
{
	 
}

CDisplayView::~CDisplayView()
{
}

BEGIN_MESSAGE_MAP(CDisplayView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDisplayView drawing

void CDisplayView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CDisplayView diagnostics

#ifdef _DEBUG
void CDisplayView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDisplayView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDisplayView message handlers


int CDisplayView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_wndMachine07.Create(IDD_MACDIS_FORM, this);
	m_wndMachine07.PictureChange(IDB_BITMAP_WIN07);
	m_wndMachine07.ShowWindow(SW_SHOW);

	m_wndMachine08.Create(IDD_MACDIS_FORM, this);
	m_wndMachine08.PictureChange(IDB_BITMAP_WIN08);
	m_wndMachine08.ShowWindow(SW_SHOW);

	m_wndMachine10.Create(IDD_MACDIS_FORM, this);
	m_wndMachine10.PictureChange(IDB_BITMAP_WIN10);
	m_wndMachine10.ShowWindow(SW_SHOW);
	return 0;
}


void CDisplayView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	m_wndMachine07.MoveWindow(025,	30, 325, cy - 60);
	m_wndMachine08.MoveWindow(365,	30, 325, cy - 60);
	m_wndMachine10.MoveWindow(705,	30, 325, cy - 60);
}


BOOL CDisplayView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


CMachineDlg& CDisplayView::getMachine07()
{
	return m_wndMachine07;
}


CMachineDlg& CDisplayView::getMachine08()
{
	return m_wndMachine08;
}


CMachineDlg& CDisplayView::getMachine10()
{
	return m_wndMachine10;
}


void CDisplayView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages

	CDC *m_mypDC = &dc;
	if (m_pOldBitmap == nullptr)
	{
		m_bitmap.CreateCompatibleBitmap(m_mypDC, MAX_SCREEN_X, MAX_SCREEN_Y);		//Make bitmap at same size.
		m_memdc.CreateCompatibleDC(m_mypDC);
	}
	m_pOldBitmap = m_memdc.SelectObject(&m_bitmap);

	CBrush backColor(RGB(MyDisplayGray, MyDisplayGray, MyDisplayGray));
	CBrush* pOldBrush = m_memdc.SelectObject(&backColor);
	CRect ViewRect;
	m_memdc.GetClipBox(&ViewRect);
	m_memdc.PatBlt(ViewRect.left, ViewRect.top, ViewRect.Width(), ViewRect.Height(), PATCOPY);
	m_memdc.SelectObject(pOldBrush);

	dc.BitBlt(0, 0, MAX_SCREEN_X, MAX_SCREEN_Y,
		&m_memdc, 0, 0, SRCCOPY);
}
