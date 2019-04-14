
// ValidateUI.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "ValidateUI.h"
#include "MainFrm.h"

#include "ValidateUIDoc.h"
#include "ValidateUIView.h"
#include "MyAppProtocol.h"
#include "DisplayView.h"
#include "ControlView.h"
#include "ControlDlg.h"
#include "MachineDlg.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CValidateUIApp

BEGIN_MESSAGE_MAP(CValidateUIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CValidateUIApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CValidateUIApp construction

CValidateUIApp::CValidateUIApp()
	:m_evtExit(FALSE, FALSE, NULL)
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("ValidateUI.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CValidateUIApp object

CValidateUIApp theApp;


// CValidateUIApp initialization

BOOL CValidateUIApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CValidateUIDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CValidateUIView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		AfxMessageBox(_T("Failed WSASStartup"));
	}
	CWinThread *pAcceptThread = AfxBeginThread(CValidateUIApp::ThreadAccept, this);
	
	AfxMessageBox(IDS_STRING_READY);

	return TRUE;
}

int CValidateUIApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	m_evtExit.SetEvent();
	Sleep(300);
	return CWinApp::ExitInstance();
}

// CValidateUIApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data   
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CValidateUIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CValidateUIApp message handlers


CValidateUIDoc* CValidateUIApp::GetValidateDoc() const
{
	return m_pDoc;
}


void CValidateUIApp::SetValidateDoc(CValidateUIDoc* pDoc)
{
	m_pDoc = pDoc;
}

UINT CValidateUIApp::ThreadAccept(LPVOID pParam)
{
	//create thread for each agent
	((CValidateUIApp*)pParam)->m_sCommand.InitCS();
	if (((CValidateUIApp*)pParam)->m_sCommand.mySocket())
		return 1;
	 
	return 0;
}

UINT CValidateUIApp::ThreadCommand(LPVOID pParam)
{
	int nReceive = 0;
	CMySocket* mySocket = (CMySocket*)pParam;
	SOCKET hClient = mySocket->Get_cSocket();

	MYCOMMAND sBuffer = { 0 };
	int nVersion = 0;

	//for connection check
	struct timeval tv = { 0 };
	tv.tv_sec = 12000;

	setsockopt(hClient, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));

	while ((nReceive = ::recv(hClient,
		(char*)&sBuffer, sizeof(sBuffer), 0)) > 0)
	{
		nVersion = sBuffer.nVersion;

		if (sBuffer.nCode != COMMAND_HEALTH && sBuffer.nCode != COMMAND_ERROR)
		{
			//create log
			MYLOG* tmpLog = new MYLOG;
			tmpLog->cNow = CTime::GetCurrentTime();
			tmpLog->nCode = sBuffer.nCode;
			tmpLog->nSize = sBuffer.nSize;
			tmpLog->nVersion = sBuffer.nVersion;

			switch (sBuffer.nVersion)
			{
			case 7:
				theApp.m_pDoc->Version07.AddTail(tmpLog);   
				theApp.m_pDisplayView->m_wndMachine07.ListInsertString(tmpLog);

				switch (sBuffer.nCode) {
				case COMMAND_READY:
					//UI Begin 
					theApp.m_pDisplayView->m_wndMachine07.m_nFile = sBuffer.nSize;
					break;
				case COMMAND_LOG_SAMPLE:
					theApp.m_pDisplayView->m_wndMachine07.m_nInfect = sBuffer.nSize;
					theApp.m_sCommand.SendCommandToOne(COMMAND_RUN_TOOL, 0, hClient);
					break;
				case COMMAND_LOG_TOOL:
					theApp.m_pDisplayView->m_wndMachine07.m_nRecovery = sBuffer.nSize;
					break;
				}
				theApp.m_pDisplayView->m_wndMachine07.InvalidateRect(NULL, 0);
				break;

			case 8:
				theApp.m_pDoc->Version08.AddTail(tmpLog);
				theApp.m_pDisplayView->m_wndMachine08.ListInsertString(tmpLog);

				switch (sBuffer.nCode) {
				case COMMAND_READY:
					theApp.m_pDisplayView->m_wndMachine08.m_nFile = sBuffer.nSize;
					break;
				case COMMAND_LOG_SAMPLE:
					theApp.m_pDisplayView->m_wndMachine08.m_nInfect = sBuffer.nSize;
					theApp.m_sCommand.SendCommandToOne(COMMAND_RUN_TOOL, 0 , hClient);
					break;
				case COMMAND_LOG_TOOL:
					theApp.m_pDisplayView->m_wndMachine08.m_nRecovery = sBuffer.nSize;
					break;
				}
				theApp.m_pDisplayView->m_wndMachine08.InvalidateRect(NULL, 0);
				break;

			case 10:
				theApp.m_pDoc->Version10.AddTail(tmpLog);
				theApp.m_pDisplayView->m_wndMachine10.ListInsertString(tmpLog);

				switch (sBuffer.nCode) {
				case COMMAND_READY:
					theApp.m_pDisplayView->m_wndMachine10.m_nFile = sBuffer.nSize;
					break;
				case COMMAND_LOG_SAMPLE:
					theApp.m_pDisplayView->m_wndMachine10.m_nInfect = sBuffer.nSize;
					theApp.m_sCommand.SendCommandToOne(COMMAND_RUN_TOOL, 0, hClient);
					break;
				case COMMAND_LOG_TOOL:
					 theApp.m_pDisplayView->m_wndMachine10.m_nRecovery = sBuffer.nSize;
					break;
				}
				theApp.m_pDisplayView->m_wndMachine10.InvalidateRect(NULL, 0);

				break;

			default:
				break;
			}
		}

		if(sBuffer.nCode == COMMAND_ERROR)
			//에러 코드 관리.

		memset((void*)&sBuffer, 0, sizeof(sBuffer));
	}


	::EnterCriticalSection(&mySocket->m_cs);		

	POSITION pos = mySocket->m_listClient.Find(hClient);
	if (pos != NULL)
		mySocket->m_listClient.RemoveAt(pos);

	::LeaveCriticalSection(&mySocket->m_cs);		
	::closesocket(hClient);

	CString myError;
	myError.Format(_T("Disconnected : WIN %d"), nVersion);
	AfxMessageBox(myError);

	return 2;
}