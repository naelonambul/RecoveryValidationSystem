#pragma once
#include "afxwin.h"
#include "VBoxController.hpp"

// CControlDlg dialog

class CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

//Attributes
public:
	int nReady = 0;
	int nMachineNumber = 0;
private:
	CComboBox m_comboOSList;

	VBox::Controller Win;

	int nIndex = 0;
public:
	CControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CControlDlg();
private:
	void ResetOSList();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonVmRun();
	afx_msg void OnBnClickedButtonSample();
	afx_msg void OnBnClickedButtonRecovery();
	afx_msg void OnBnClickedButtonVmReset();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonSendfile();
	void AddReady();
	BOOL IsReady();
};
