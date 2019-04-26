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
	pVboxThread->PostThreadMessage(WM_USER_STOP, NULL, NULL);
	WaitForSingleObject(waitMsg, 300);

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

	//Create Event for Sync
	waitMsg = CreateEvent(NULL, TRUE, FALSE, NULL);

	//Create Thread for Vbox
	pVboxThread = AfxBeginThread(
		CControlDlg::ThreadBox,
		this,
		NULL,
		NULL,
		NULL,
		NULL);

	pVboxThread->PostThreadMessage(WM_USER_VMREADY, NULL, NULL);
	WaitForSingleObject(waitMsg, INFINITE);

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

		pVboxThread->PostThreadMessage(WM_USER_VMSTOP, NULL, NULL);
		WaitForSingleObject(waitMsg, INFINITE);

		pVboxThread->PostThreadMessage(WM_USER_VMSTART, NULL, NULL);
		WaitForSingleObject(waitMsg, INFINITE);
	
		if (myCurSel == _T("IE11 - Win7")) {
			theApp.m_pDisplayView->m_wndMachine07.PostMessage(WM_USER_SPINSTART, NULL, READY);
		}
		else if (myCurSel == _T("IE11 - Win81")) {
			theApp.m_pDisplayView->m_wndMachine08.PostMessage(WM_USER_SPINSTART, NULL, READY);
		}
		else if (myCurSel == _T("MSEdge - Win10")) {
			theApp.m_pDisplayView->m_wndMachine10.PostMessage(WM_USER_SPINSTART, NULL, READY);
		}
	}
	Sleep(900);
	if (nReady >= 3){	GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);	}
	else { GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE); }
}


void CControlDlg::OnBnClickedButtonSendfile()
{
	// TODO: Add your control notification handler code here

	CString StartString;

	if (theApp.m_pDoc->GetSamplePath() 	!= _T("") ||
		theApp.m_pDoc->GetToolPath() != _T("")
		&& nReady) {
		GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);

		theApp.m_sCommand.SendCommandToAll(COMMAND_SND_SAMPLE, 0);
		theApp.m_sCommand.SendFileToAll(theApp.m_pDoc->GetSamplePath());
		Sleep(1000);
		theApp.m_sCommand.SendCommandToAll(COMMAND_SND_TOOL, 0);
		theApp.m_sCommand.SendFileToAll(theApp.m_pDoc->GetToolPath());
		Sleep(1000);
		theApp.m_sCommand.SendCommandToAll(COMMAND_RUN_SAMPLE, 0);

		if (n07Ready == 1) {
			theApp.m_pDisplayView->m_wndMachine07.PostMessage(WM_USER_SPINSTART, NULL, SAMPLE);
		}		
		if (n08Ready == 1) {
			theApp.m_pDisplayView->m_wndMachine08.PostMessage(WM_USER_SPINSTART, NULL, SAMPLE);
		}		
		if (n10Ready == 1) {
			theApp.m_pDisplayView->m_wndMachine10.PostMessage(WM_USER_SPINSTART, NULL, SAMPLE);
		}

		if (nReady >= 3) {
			GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);
		}
	}
	else if (!Win.nRunning) {
		StartString.LoadStringW(IDS_STRING_VM_RUNNING);
		AfxMessageBox(StartString);
	}
	else {
		StartString.LoadStringW(IDS_STRING_FILEPATH);
	AfxMessageBox(StartString);
	}
}


void CControlDlg::OnBnClickedButtonVmReset()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(FALSE);
	theApp.m_sCommand.SendCommandToAll(COMMAND_STOP, 0);
	Sleep(1000);

	pVboxThread->PostThreadMessage(WM_USER_VMEXIT, NULL, NULL);
	WaitForSingleObject(waitMsg, INFINITE);

	pVboxThread->PostThreadMessage(WM_USER_VMREADY, NULL, NULL);
	WaitForSingleObject(waitMsg, INFINITE);

	ResetOSList();

	GetDlgItem(IDC_BUTTON_VMRUN)->EnableWindow(TRUE);

	theApp.m_pDisplayView->m_wndMachine07.PostMessage(WM_USER_SPINSTOP, NULL, ALL);
	theApp.m_pDisplayView->m_wndMachine08.PostMessage(WM_USER_SPINSTOP, NULL, ALL);
	theApp.m_pDisplayView->m_wndMachine10.PostMessage(WM_USER_SPINSTOP, NULL, ALL);

	nReady = 0;//reset count
	n07Ready = 0;
	n08Ready = 0;
	n10Ready = 0;
}


void CControlDlg::ResetOSList()
{
	m_comboOSList.ResetContent();

	map<string, string> ::iterator PrintIter;
	if (Win.mapOS.empty() == FALSE)	{
		for (PrintIter = Win.mapOS.begin();	PrintIter != Win.mapOS.end(); PrintIter++) {
			wstring printBuffer = wstring(	PrintIter->first.begin(), PrintIter->first.end());
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
	return nReady == Win.nRunMachine;
}


afx_msg LRESULT CControlDlg::OnUser07ready(WPARAM wParam, LPARAM lParam)
{
	n07Ready = (int)lParam;

	AddReady();
	if (IsReady()) { GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE); }
	else { GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE); }

	return 0;
}


afx_msg LRESULT CControlDlg::OnUser08ready(WPARAM wParam, LPARAM lParam)
{
	n08Ready = (int)lParam;

	AddReady();
	if (IsReady()) { GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE); }
	else { GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE); }
	return 0;
}


afx_msg LRESULT CControlDlg::OnUser10ready(WPARAM wParam, LPARAM lParam)
{
	n10Ready = (int)lParam;

	AddReady();
	if (IsReady()) { GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(TRUE); }
	else { GetDlgItem(IDC_BUTTON_SENDFILE)->EnableWindow(FALSE); }
	return 0;
}


UINT CControlDlg::ThreadBox(LPVOID pParam)
{
	CControlDlg* pDlg = (CControlDlg*)pParam;

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		switch (msg.message)
		{
		case WM_USER_VMREADY: 
			pDlg->Win.GetProgramFilesPath();
			pDlg->Win.Run(VMLIST);
			SetEvent(pDlg->waitMsg);
			break;
		case WM_USER_VMSTOP: 
			pDlg->Win.Run(VMSTOP);
			SetEvent(pDlg->waitMsg);
			break;
		case WM_USER_VMSTART: 
			pDlg->Win.Run(VMSTART);
			SetEvent(pDlg->waitMsg);
			break;
		case WM_USER_VMEXIT: 
			pDlg->Win.exitVM();
			SetEvent(pDlg->waitMsg);
			break;
		case WM_USER_VMEXITONE:
			pDlg->Win.exitOneVm((int)msg.lParam);
			break;
		case WM_USER_STOP:  
			SetEvent(pDlg->waitMsg);
			return 0;
		default:
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return 3;
}