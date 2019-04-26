
// ValidateUI.h : main header file for the ValidateUI application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "MyAppProtocol.h"
#include "MySocket.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#include <windows.h>
#include <list>
#include <iterator>
#include "afxcoll.h"
#include "afxmt.h"
#include "ValidateUIDoc.h"

class CSampleFileView;
class CRecoveryFileView;
class CDisplayView;
class CControlView;

enum g_DLGCOLOR {
	MyDlgGray = 240,
	defaultGray = 221,
	MyDisplayGray = 240
};

enum g_MYFNTSIZE {
	Insertfntsize = 40,
	FilePathfntsize = 40
};

enum g_SCREENSIZE {
	CxLength = 1400,
	CyLength = 800,

	CxFileDlg = 175,
	CyFileDlg = (CyLength - 50) / 2,
	CxControlDlg = 175,

	CxDisplay = CxLength - CxFileDlg - CxControlDlg
};

enum g_SPINPOSITION {
	READY	= 100,
	SAMPLE	= 200,
	TOOL	= 300,
	ALL		= 400
};

// CValidateUIApp:
// See ValidateUI.cpp for the implementation of this class
//

class CValidateUIApp : public CWinApp
{
public:
	CValidateUIApp();

//Attributes
public:
	CSampleFileView* m_pSampleFileView = nullptr;
	CRecoveryFileView* m_pRecoveryFileView = nullptr;

	CDisplayView* m_pDisplayView = nullptr;
	CControlView* m_pControlView = nullptr;

	CValidateUIDoc* m_pDoc = nullptr;
	
	CMySocket m_sCommand;

	CRITICAL_SECTION m_csCM;

	CEvent m_evtExit;
protected:

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	static UINT ThreadCommand(LPVOID pParam);
	static UINT ThreadAccept(LPVOID pParam);

	CValidateUIDoc* GetValidateDoc() const;
	void SetValidateDoc(CValidateUIDoc* pDoc);
};

extern CValidateUIApp theApp;
