#pragma once
#include "afxwin.h"
#include "VBoxController.hpp"

// CControlDlg dialog

using ::VBox::Controller;

class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

//Attributes
public:
	int nReady = 0;	//working state

	int n07Ready = 0;
	int n08Ready = 0;
	int n10Ready = 0;

	int nMachineNumber = 0;

	Controller VBoxManager;

	HANDLE waitMsg = NULL;

	CWinThread *pVboxThread = nullptr;
private:
	CComboBox m_comboOSList;
	
	int nIndex = 0;
public:
	CControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlDlg();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnUserReady(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonVmRun();
	afx_msg void OnBnClickedButtonVmReset();
	afx_msg void OnBnClickedButtonSendfile();
	void readyCount(BOOL bState);
	BOOL IsReady();

	static UINT ThreadBox(LPVOID pParam);
private:
	void ResetOSList();
};
