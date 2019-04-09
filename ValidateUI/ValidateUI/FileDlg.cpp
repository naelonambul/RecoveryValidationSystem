// FileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "MainFrm.h"
#include "ValidateUIDoc.h"
#include "FileDlg.h"
#include "afxdialogex.h"

#define MAX_SCREEN_X 1920
#define MAX_SCREEN_Y 1080
// CFileDlg dialog

IMPLEMENT_DYNAMIC(CFileDlg, CDialogEx)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILE_FORM, pParent)
{

}

CFileDlg::~CFileDlg()
{
}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialogEx)
	ON_WM_DROPFILES()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CFileDlg message handlers


BOOL CFileDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			pMsg->wParam = 0;
		}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CFileDlg::PictureChange(int IDB_NUM)
{
	CStatic *m_pPicture1 = (CStatic*)GetDlgItem(IDC_PICVIEW);

	new_image.LoadBitmapW(IDB_NUM);
	HBITMAP h_old_bitmap = m_pPicture1->SetBitmap(new_image);
	if (h_old_bitmap != NULL)
		::DeleteObject(h_old_bitmap);
	
	new_image.Detach();
}


void CFileDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	PictureChange(IDB_BITMAP_COMPLETE);
	DWORD nBuffer = 0;
	CString sFile;

	nBuffer = ::DragQueryFile(hDropInfo, 0, NULL, 0);
	DragQueryFile(hDropInfo, 0, sFile.GetBuffer(nBuffer + 1), nBuffer + 1);

	if (m_pParentView == (CView*)theApp.m_pSampleFileView)
		theApp.GetValidateDoc()->SetSamplePath(sFile);
	else if (m_pParentView == (CView*)theApp.m_pRecoveryFileView)
		theApp.GetValidateDoc()->SetToolPath(sFile);
	m_bottomString = sFile;
	InvalidateRect(NULL, 0);
	CDialogEx::OnDropFiles(hDropInfo);
}


void CFileDlg::SetPen()
{
	m_FileDlgpen.CreatePen(PS_SOLID, 15, RGB(0, 0, 0));
}


int CFileDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	SetPen();
	return 0;
}


BOOL CFileDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default


	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}


void CFileDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	CDC *mypDC = &dc;
	if (pOldBitmap == nullptr)
	{
		bitmap.CreateCompatibleBitmap(mypDC, MAX_SCREEN_X, MAX_SCREEN_Y);		//Make bitmap at same size.
		memdc.CreateCompatibleDC(mypDC);
	}
	pOldBitmap = memdc.SelectObject(&bitmap);

	CBrush backColor(RGB(MyDisplayGray, MyDisplayGray, MyDisplayGray));
	CBrush* pOldBrush = memdc.SelectObject(&backColor);
	CRect ViewRect;
	memdc.GetClipBox(&ViewRect);
	memdc.PatBlt(ViewRect.left, ViewRect.top, ViewRect.Width(), ViewRect.Height(), PATCOPY);
	memdc.SelectObject(pOldBrush);
	memdc.SelectObject(m_FileDlgpen);
	memdc.SetBkMode(TRANSPARENT);
	memdc.TextOutW(20, 20, m_bottomString);


	dc.BitBlt(0, 0, MAX_SCREEN_X, MAX_SCREEN_Y,
		&memdc, 0, 0, SRCCOPY);
}
