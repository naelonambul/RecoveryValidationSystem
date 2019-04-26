#pragma once
#include "afxwin.h"
#include "PictureEx.h"

struct MYLOG;
// CMachineDlg dialog

class CMachineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMachineDlg)

private:
	CBitmap new_image;

	CPen m_FileDlgpen;
	CString m_bottomString;

	CDC memdc;									//Double buffer
	CDC *mypDC = nullptr;						//Only OnPaint

	CBitmap *pOldBitmap = nullptr;
	CBitmap bitmap;

	int m_nCount;
	CListBox m_Log_List;


public:
	CMachineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMachineDlg();

	int m_nFile = 0;
	int m_nInfect = 0;
	int m_nRecovery = 0;

	CPictureEx m_PictureTop;
	CPictureEx m_PictureMid;
	CPictureEx m_PictureBtm;
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
	void CMachineDlg::ListInsertString(MYLOG* const tmpLog);
	virtual BOOL OnInitDialog();


protected:
	afx_msg LRESULT OnUserSpinstart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserSpinstop(WPARAM wParam, LPARAM lParam);
};
