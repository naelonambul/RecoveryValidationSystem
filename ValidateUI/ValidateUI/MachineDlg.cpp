// MachineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "MachineDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#define MAX_SCREEN_X 1920
#define MAX_SCREEN_Y 1080
#define HIMETRIC_INCH        2540
// CMachineDlg dialog

IMPLEMENT_DYNAMIC(CMachineDlg, CDialogEx)

CMachineDlg::CMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MACDIS_FORM, pParent)
{
}

CMachineDlg::~CMachineDlg()
{
}

void CMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LOG, m_LogList);
	DDX_Control(pDX, IDC_MYPICTURETOP, m_PictureTop);
	DDX_Control(pDX, IDC_MYPICTUREMID, m_PictureMid);
	DDX_Control(pDX, IDC_MYPICTUREBTM, m_PictureBtm);
}


BEGIN_MESSAGE_MAP(CMachineDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_MESSAGE(WM_USER_SPINSTART, &CMachineDlg::OnUserSpinstart)
	ON_MESSAGE(WM_USER_SPINSTOP, &CMachineDlg::OnUserSpinstop)
	ON_MESSAGE(WM_USER_LOGPRINT, &CMachineDlg::OnUserLogprint)
END_MESSAGE_MAP()


// CMachineDlg message handlers


BOOL CMachineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CMachineDlg::PictureChange(int IDB_NUM)
{
	CStatic *m_pPictureOS = (CStatic*)GetDlgItem(IDC_PICMACDIS);

	m_newimage.LoadBitmapW(IDB_NUM);
	HBITMAP h_old_bitmap = m_pPictureOS->SetBitmap(m_newimage);
	if (h_old_bitmap != NULL)
		::DeleteObject(h_old_bitmap);

	m_newimage.Detach();
}


BOOL CMachineDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}

void CMachineDlg::OnPaint()
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

	m_memdc.FillSolidRect(0, 0,
		MAX_SCREEN_X, MAX_SCREEN_Y,
		RGB(255, 255, 255));

	m_memdc.SetBkColor(RGB(255, 255, 255));

	CString strFile;
	strFile.LoadStringW(IDS_STRING_NUMFILE);
	strFile.AppendFormat(_T("%d"), m_nFile);
	m_memdc.TextOut(50, 100, strFile);

	CString strInfect;
	strInfect.LoadStringW(IDS_STRING_INFECTFILE);
	strInfect.AppendFormat(_T("%d"),m_nInfect);
	m_memdc.TextOut(50, 200, strInfect);

	CString strRecovery;
	strRecovery.LoadStringW(IDS_STRING_RECOVEFILE);
	strRecovery.AppendFormat(_T("%d"), m_nRecovery);
	m_memdc.TextOut(50, 300, strRecovery);

	dc.BitBlt(0, 0, MAX_SCREEN_X, MAX_SCREEN_Y,
		&m_memdc, 0, 0, SRCCOPY);

}
void CMachineDlg::ListGetCount()
{
	m_nListBoxCount = m_LogList.GetCount();
}


void CMachineDlg::ListInsertString(MYLOG* const tmpLog)
{

	CString CommandString;
	switch (tmpLog->nCode) {
	case COMMAND_HEALTH:		CommandString.LoadString(IDS_STRING_HEALTH); break;
	case COMMAND_ERROR: 		CommandString.LoadString(IDS_STRING_ERROR); break;
	case COMMAND_SND_SAMPLE:	CommandString.LoadString(IDS_STRING_SND_SAMPLE); break;
	case COMMAND_SND_TOOL:		CommandString.LoadString(IDS_STRING_SND_TOOL); break;
	case COMMAND_BEGIN_FILE:	CommandString.LoadString(IDS_STRING_BEGIN_FILE); break;
	case COMMAND_END_FILE:		CommandString.LoadString(IDS_STRING_END_FILE); break;
	case COMMAND_RUN_SAMPLE:	CommandString.LoadString(IDS_STRING_RUN_SAMPLE); break;
	case COMMAND_END_SAMPLE:	CommandString.LoadString(IDS_STRING_END_SAMPLE); break;
	case COMMAND_LOG_SAMPLE:	CommandString.LoadString(IDS_STRING_LOG_SAMPLE); break;
	case COMMAND_RUN_TOOL:		CommandString.LoadString(IDS_STRING_RUN_TOOL); break;
	case COMMAND_END_TOOL:		CommandString.LoadString(IDS_STRING_END_TOOL); break;
	case COMMAND_LOG_TOOL:		CommandString.LoadString(IDS_STRING_LOG_TOOL); break;
	case COMMAND_STOP:			CommandString.LoadString(IDS_STRING_STOP); break;
	case COMMAND_READY:			CommandString.LoadString(IDS_STRING_AGENTREADY); break;
	}
	//push data doc
	CTime timeBuffer = tmpLog->cNow;
	CString strDataTime;
	strDataTime = timeBuffer.Format(_T("%Y년%m월%d일 - %I:%M:%S "));
	CommandString.Insert(0, strDataTime);

	ListGetCount();
	m_LogList.InsertString(m_nListBoxCount, CommandString);
	m_LogList.SetCurSel(m_nListBoxCount);
}


afx_msg LRESULT CMachineDlg::OnUserSpinstart(WPARAM wParam, LPARAM lParam)
{
	switch((int)lParam)
	{
	case READY:
		m_PictureTop.Load(MAKEINTRESOURCE(IDR_GIF6), _T("GIF"));
		m_PictureTop.Draw();
		break;
	case SAMPLE:
		m_PictureMid.Load(MAKEINTRESOURCE(IDR_GIF6), _T("GIF"));
		m_PictureMid.Draw();
		break;
	case TOOL:
		m_PictureBtm.Load(MAKEINTRESOURCE(IDR_GIF6), _T("GIF"));
		m_PictureBtm.Draw();
		break;
	}
return 0;
}


afx_msg LRESULT CMachineDlg::OnUserSpinstop(WPARAM wParam, LPARAM lParam)
{
	switch ((int)lParam)
	{
	case READY:
		m_PictureTop.UnLoad();
		Sleep(1);
		break;
	case SAMPLE:
		m_PictureMid.UnLoad();
		Sleep(1);
		break;
	case TOOL:
		m_PictureBtm.UnLoad();
		Sleep(1);
		break;
	case ALL:
		PostMessage(WM_USER_SPINSTOP, NULL, READY);
		PostMessage(WM_USER_SPINSTOP, NULL, SAMPLE);
		PostMessage(WM_USER_SPINSTOP, NULL, TOOL);
	}
	InvalidateRect(NULL, 0);
	return 0;
}


afx_msg LRESULT CMachineDlg::OnUserLogprint(WPARAM wParam, LPARAM lParam)
{
	MYLOG* tmpLog = (MYLOG*)lParam;

	CString CommandString;
	switch (tmpLog->nCode) {
	case COMMAND_HEALTH:		CommandString.LoadString(IDS_STRING_HEALTH); break;
	case COMMAND_ERROR: 		CommandString.LoadString(IDS_STRING_ERROR); break;
	case COMMAND_SND_SAMPLE:	CommandString.LoadString(IDS_STRING_SND_SAMPLE); break;
	case COMMAND_SND_TOOL:		CommandString.LoadString(IDS_STRING_SND_TOOL); break;
	case COMMAND_BEGIN_FILE:	CommandString.LoadString(IDS_STRING_BEGIN_FILE); break;
	case COMMAND_END_FILE:		CommandString.LoadString(IDS_STRING_END_FILE); break;
	case COMMAND_RUN_SAMPLE:	CommandString.LoadString(IDS_STRING_RUN_SAMPLE); break;
	case COMMAND_END_SAMPLE:	CommandString.LoadString(IDS_STRING_END_SAMPLE); break;
	case COMMAND_LOG_SAMPLE:	CommandString.LoadString(IDS_STRING_LOG_SAMPLE); break;
	case COMMAND_RUN_TOOL:		CommandString.LoadString(IDS_STRING_RUN_TOOL); break;
	case COMMAND_END_TOOL:		CommandString.LoadString(IDS_STRING_END_TOOL); break;
	case COMMAND_LOG_TOOL:		CommandString.LoadString(IDS_STRING_LOG_TOOL); break;
	case COMMAND_STOP:			CommandString.LoadString(IDS_STRING_STOP); break;
	case COMMAND_READY:			CommandString.LoadString(IDS_STRING_AGENTREADY); break;
	}

	CTime timeBuffer = tmpLog->cNow;
	CString strDataTime;
	strDataTime = timeBuffer.Format(_T("%Y년%m월%d일 - %I:%M:%S "));
	CommandString.Insert(0, strDataTime);

	ListGetCount();
	m_LogList.InsertString(m_nListBoxCount, CommandString);
	m_LogList.SetCurSel(m_nListBoxCount);
	return 0;
}
