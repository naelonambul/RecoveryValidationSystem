#pragma once


// CFileDlg dialog

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

//Attributes
public:

	CView* m_pParentView;
private:
	CBitmap new_image;
	
	CFont m_dlgFont;
	LOGFONT m_dlgLf;

	CFont* m_pOldFont;

	CString m_bottomString;
	CString m_Hash;

	CDC memdc;
	CDC*mypDc = nullptr;

	CBitmap *pOldBitmap = nullptr;
	CBitmap bitmap;

public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	void PictureChange(int IDB_NUM);
	void SetPen();
};
