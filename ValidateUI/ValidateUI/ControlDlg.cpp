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
	ON_BN_CLICKED(IDC_BUTTON_SAMPLE, &CControlDlg::OnBnClickedButtonSample)
	ON_BN_CLICKED(IDC_BUTTON_RECOVERY, &CControlDlg::OnBnClickedButtonRecovery)
	ON_BN_CLICKED(IDC_BUTTON_VMRESET, &CControlDlg::OnBnClickedButtonVmReset)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CControlDlg::OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_SENDFILE, &CControlDlg::OnBnClickedButtonSendfile)
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

	m_comboOSList.DeleteString(nIndex);

	//GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE);

	//GetDlgItem(IDC_BUTTON_SAMPLE)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(FALSE);
	//
	//GetDlgItem(IDC_BUTTON_VMRESET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);
	Win.SetCurrentOS(myCurSel);
	Win.Run(VMSTOP);
	Sleep(100);

	Win.Run(VMSTART);
	Sleep(100);
}


void CControlDlg::OnBnClickedButtonSendfile()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE);

	//GetDlgItem(IDC_BUTTON_SAMPLE)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_VMRESET)->EnableWindow(TRUE);
	//
	//GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);

	if (theApp.m_pDoc->GetSamplePath() != "" &&
		theApp.m_pDoc->GetToolPath() != "") {
		theApp.m_sCommand.SendCommand(COMMAND_SND_SAMPLE, 0);
		theApp.m_sCommand.SendFile(theApp.m_pDoc->GetSamplePath());
		Sleep(1000);
		theApp.m_sCommand.SendCommand(COMMAND_SND_TOOL, 0);
		theApp.m_sCommand.SendFile(theApp.m_pDoc->GetToolPath());
		Sleep(1000);
		theApp.m_sCommand.SendCommand(COMMAND_RUN_SAMPLE, 0);
		Sleep(1000);
		theApp.m_sCommand.SendCommand(COMMAND_RUN_TOOL, 0);
	}
	else
		AfxMessageBox(_T("파일 경로를 확인해 주세요."));
	//SetEvent(theApp.m_sCommand.fileEvent);

}

void CControlDlg::OnBnClickedButtonSample()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_SAMPLE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(TRUE);
	//
	//GetDlgItem(IDC_BUTTON_VMRESET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);


}


void CControlDlg::OnBnClickedButtonRecovery()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_SAMPLE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(FALSE);
	//
	//GetDlgItem(IDC_BUTTON_VMRESET)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);


}


void CControlDlg::OnBnClickedButtonVmReset()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_SAMPLE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_VMRESET)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(TRUE);

	ResetOSList();
	Win.exitVM();
}


void CControlDlg::OnBnClickedButtonExport()
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE);
	//GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_SAMPLE)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_RECOVERY)->EnableWindow(FALSE);

	//GetDlgItem(IDC_BUTTON_VMRESET)->EnableWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_EXPORT)->EnableWindow(FALSE);

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
