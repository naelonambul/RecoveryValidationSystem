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
#define HIMETRIC_INCH 2540
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

	m_newimage.LoadBitmapW(IDB_NUM);
	HBITMAP h_old_bitmap = m_pPicture1->SetBitmap(m_newimage);
	if (h_old_bitmap != NULL)
		::DeleteObject(h_old_bitmap);

	m_newimage.Detach();
}


void CFileDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: Add your message handler code here and/or call default
	DWORD nBuffer = 0;
	TCHAR pszFilePath[_MAX_FNAME];
	TCHAR pszFileName[_MAX_FNAME];
	TCHAR pszFileExt[_MAX_EXT];

	nBuffer = ::DragQueryFile(hDropInfo, 0, NULL, 0);
	DragQueryFile(hDropInfo, 0, pszFilePath, sizeof(pszFilePath));
	_tsplitpath_s(pszFilePath,
		NULL, NULL,
		NULL, NULL,
		pszFileName, _MAX_FNAME,
		pszFileExt, _MAX_EXT);

	CString sFilePath(pszFilePath);
	CString sFileName(pszFileName);
	sFileName.Append(pszFileExt);

	//exe filter
	if (_tcscmp(pszFileExt, _T(".exe")) == 0)	{
		if (sampleOrTool())
			theApp.GetValidateDoc()->SetSamplePath(sFilePath);
		else 
			theApp.GetValidateDoc()->SetToolPath(sFilePath);

		PictureChange(IDB_BITMAP_COMPLETE);
		m_bottomString = sFileName;
		
		InvalidateRect(NULL, 0);
	}
	else	{
		CString ErrorFilePath;
		ErrorFilePath.LoadStringW(IDS_STRING_FILEPATH);
		AfxMessageBox(ErrorFilePath);

	}
	CDialogEx::OnDropFiles(hDropInfo);
}


void CFileDlg::SetPen()
{
	GetFont()->GetLogFont(&m_dlgLf);
	m_dlgLf.lfHeight = 15;
	m_dlgFont.CreateFontIndirect(&m_dlgLf);
}


int CFileDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

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
	m_memdc.PatBlt(ViewRect.left, ViewRect.top, 
		ViewRect.Width(), ViewRect.Height(), PATCOPY);
	m_memdc.SelectObject(pOldBrush);
	m_memdc.SetBkMode(TRANSPARENT);

	if (m_bottomString == "") 
	{
		SetPen();
		m_pOldFont = m_memdc.SelectObject(&m_dlgFont);

		CString GuideString;

		if (sampleOrTool())
			GuideString.LoadStringW(IDS_STRING_DROPSAMPLE);
		else
			GuideString.LoadStringW(IDS_STRING_DROPTOOL);

		m_bottomString = GuideString;
	}
	m_memdc.TextOut(15, 23, m_bottomString);
	m_memdc.SelectObject(m_pOldFont);
	dc.BitBlt(0, 0, MAX_SCREEN_X, MAX_SCREEN_Y,
		&m_memdc, 0, 0, SRCCOPY);
}


BOOL CFileDlg::sampleOrTool()
{
	if (m_pParentView == (CView*)theApp.m_pSampleFileView)
		return 1;
	else
		return 0;

	return 0;
}
