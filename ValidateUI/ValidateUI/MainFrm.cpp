
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "SampleFileView.h"
#include "RecoveryFileView.h"
#include "DisplayView.h"
#include "ControlView.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("Failed to create status bar\n");
	//	return -1;      // fail to create
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	//// TODO: Delete these three lines if you don't want the toolbar to be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_SYSMENU;

	cs.style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

	cs.cx = CxLength;
	cs.cy = CyLength;

	cs.hMenu = NULL;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	SetInsertfnt(_T("consolas"));
	SetFilePathfnt(_T("Tahoma"));

	if(!m_wndSplitHor.CreateStatic(this, 1, 3))
		return FALSE;

	if (!m_wndSplitVer.CreateStatic(&m_wndSplitHor, 2, 1,
		WS_CHILD | WS_VISIBLE,
		m_wndSplitHor.IdFromRowCol(0, 0)))
		return FALSE;

	if (!m_wndSplitVer.CreateView(0, 0,
		RUNTIME_CLASS(CSampleFileView), CSize(CxFileDlg, CyFileDlg - 10), pContext))
		return FALSE;

	if (!m_wndSplitVer.CreateView(1, 0,
		RUNTIME_CLASS(CRecoveryFileView), CSize(CxFileDlg  , CyFileDlg), pContext))
		return FALSE;


	m_wndSplitHor.SetColumnInfo(0, CxFileDlg, CxFileDlg);

	if (!m_wndSplitHor.CreateView(0, 1,
		RUNTIME_CLASS(CDisplayView), CSize(CxDisplay, CyLength), pContext))
		return FALSE;

	if (!m_wndSplitHor.CreateView(0, 2,
		RUNTIME_CLASS(CControlView), CSize(CxControlDlg, CyLength), pContext))
		return FALSE;


	theApp.m_pSampleFileView = (CSampleFileView*)m_wndSplitVer.GetPane(0, 0);
	theApp.m_pRecoveryFileView = (CRecoveryFileView*)m_wndSplitVer.GetPane(1, 0);
	
	theApp.m_pDisplayView = (CDisplayView*)m_wndSplitHor.GetPane(0, 1);
	theApp.m_pControlView = (CControlView*)m_wndSplitHor.GetPane(0, 2);
	
	SetActiveView((CView*)m_wndSplitVer.GetPane(0, 0));

	return TRUE;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::SetInsertfnt(TCHAR* pszfntName)
{
	wsprintf(m_Insertlf.lfFaceName, _T("%s"), pszfntName);
	m_Insertlf.lfHeight = Insertfntsize;							//fix Font size

	m_Insertfnt.CreateFontIndirect(&m_Insertlf);
}


void CMainFrame::SetFilePathfnt(TCHAR* pszfntName)
{
	wsprintf(m_FilePathlf.lfFaceName, _T("%s"), pszfntName);
	m_FilePathlf.lfHeight = FilePathfntsize;							//fix Font size

	m_FilePathfnt.CreateFontIndirect(&m_FilePathlf);
}
