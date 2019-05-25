#pragma once
#include "afxwin.h"
#include "PictureEx.h"

struct MYLOG;
// CMachineDlg dialog

class CMachineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineDlg)

private:
	CBitmap m_newimage;
	CString m_topString;

	CDC  m_memdc;								//Double buffer
	CDC* m_mypDC = nullptr;						//Only OnPaint

	CBitmap* m_pOldBitmap = nullptr;
	CBitmap  m_bitmap;

	int m_nListBoxCount;
	CListBox m_LogList;

public:
	CMachineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMachineDlg();

	int m_nFile = 0;
	int m_nInfect = 0;
	int m_nRecovery = 0;

	CPictureEx m_PictureTop;				//for vm loading
	CPictureEx m_PictureMid;				//for sample loading
	CPictureEx m_PictureBtm;				//for tool loading
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MACDIS_FORM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	void PictureChange(int IDB_NUM);
	void ListGetCount();
	void ListInsertString(MYLOG* const tmpLog);
	virtual BOOL OnInitDialog();

protected:
	afx_msg LRESULT OnUserSpinstart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserSpinstop(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserLogprint(WPARAM wParam, LPARAM lParam);
};
