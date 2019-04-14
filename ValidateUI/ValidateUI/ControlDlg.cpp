// ControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "ControlDlg.h"
#include "afxdialogex.h"
#include <cstdlib>

using std::basic_string;   
typedef basic_string<wchar_t> wstring;
// CControlDlg dialog

IMPLEMENT_DYNAMIC(CControlDlg, CDialogEx)

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONTROLDLG, pParent)
{

}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECTOS, m_comboOSList);
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_VMRUN, &CControlDlg::OnBnClickedButtonVmRun)
	ON_BN_CLICKED(IDC_BUTTON_VMRESET, &CControlDlg::OnBnClickedButtonVmReset)
	ON_BN_CLICKED(IDC_BUTTON_SENDFILE, &CControlDlg::OnBnClickedButtonSendfile)
	ON_MESSAGE(WM_USER_07READY, &CControlDlg::OnUser07ready)
	ON_MESSAGE(WM_USER_08READY, &CControlDlg::OnUser08ready)
	ON_MESSAGE(WM_USER_10READY, &CControlDlg::OnUser10ready)
END_MESSAGE_MAP()


// CControlDlg message handlers


BOOL CControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN)
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		{
			pMsg->wParam = 0;
		}

	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	Win.GetProgramFilesPath();
	Win.Run(VMLIST);
	ResetOSList();
	m_comboOSList.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CControlDlg::OnBnClickedButtonVmRun()
{
	// TODO: Add your control notification handler code here
	CString myCurSel;
	m_comboOSList.GetWindowText(myCurSel);
	nIndex = m_comboOSList.GetCurSel();

	if (nIndex != CB_ERR)	{
		m_comboOSList.DeleteString(nIndex);

		GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);

		Win.SetCurrentOS(myCurSel);
		Win.Run(VMSTOP);
		Sleep(1000);

		Win.Run(VMSTART);
		Sleep(1000);
		GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE);
		//준비된거 확인후.
		GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE);
	}
}


void CControlDlg::OnBnClickedButtonSendfile()
{
	// TODO: Add your control notification handler code here

	if (theApp.m_pDoc->GetSamplePath() != "" ||
		theApp.m_pDoc->GetToolPath() != "" && Win.nRunning) {
		GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);

		theApp.m_sCommand.SendCommandToAll(COMMAND_SND_SAMPLE, 0);
		theApp.m_sCommand.SendFileToAll(theApp.m_pDoc->GetSamplePath());
		Sleep(1000);
		theApp.m_sCommand.SendCommandToAll(COMMAND_SND_TOOL, 0);
		theApp.m_sCommand.SendFileToAll(theApp.m_pDoc->GetToolPath());
		Sleep(1000);
		theApp.m_sCommand.SendCommandToAll(COMMAND_RUN_SAMPLE, 0);
		if(nReady == 3)
			GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
	}
	else if (!Win.nRunning) {
		AfxMessageBox(_T("가상머신이 작동중입니다."));
	}
	else
		AfxMessageBox(_T("파일 경로를 확인해 주세요."));

}


void CControlDlg::OnBnClickedButtonVmReset()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);

	ResetOSList();
	Win.exitVM();

	GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE);
	theApp.m_pDoc->exportCSV();
}


void CControlDlg::ResetOSList()
{
	m_comboOSList.ResetContent();

	map<string, string> ::iterator PrintIter;
	if (Win.mapOS.empty() == FALSE)
	{
		for (PrintIter = Win.mapOS.begin();
			PrintIter != Win.mapOS.end();
			PrintIter++) {
			wstring printBuffer = wstring(
				PrintIter->first.begin(), PrintIter->first.end());
			const wchar_t* result = printBuffer.c_str();
			m_comboOSList.AddString(result);
		}
	}
}

void CControlDlg::AddReady()
{
	if( nReady < 3 && nReady >= 0)
	++nReady;
}

BOOL CControlDlg::IsReady()
{
	return nReady == 3;
}


afx_msg LRESULT CControlDlg::OnUser07ready(WPARAM wParam, LPARAM lParam)
{
	if (n07Ready) n07Ready = 0;
	else n07Ready = 1;

	AddReady();
	return 0;
}


afx_msg LRESULT CControlDlg::OnUser08ready(WPARAM wParam, LPARAM lParam)
{
	if (n08Ready) n08Ready = 0;
	else n08Ready = 1;

	AddReady();
	return 0;
}


afx_msg LRESULT CControlDlg::OnUser10ready(WPARAM wParam, LPARAM lParam)
{
	if (n10Ready) n10Ready = 0;
	else n10Ready = 1;

	AddReady();
	return 0;
}
