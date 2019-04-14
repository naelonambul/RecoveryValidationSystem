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

	int n07Ready = 0;
	int n08Ready = 0;
	int n10Ready = 0;


	int nMachineNumber = 0;

	VBox::Controller Win;

private:
	CComboBox m_comboOSList;


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
	afx_msg void OnBnClickedButtonVmReset();
	afx_msg void OnBnClickedButtonSendfile();
	void AddReady();
	BOOL IsReady();
protected:
	afx_msg LRESULT OnUser07ready(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser08ready(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser10ready(WPARAM wParam, LPARAM lParam);
};
