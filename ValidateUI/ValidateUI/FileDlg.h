#pragma once


// CFileDlg dialog

class CFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileDlg)

//Attributes
public:
	CView*		m_pParentView;
protected:
private:
	CBitmap		m_newimage;
	
	CFont		m_dlgFont;
	LOGFONT		m_dlgLf;

	CFont*		m_pOldFont = nullptr;;

	CString		m_bottomString;

	CDC			m_memdc;
	CDC*		m_mypDC = nullptr;

	CBitmap*	m_pOldBitmap = nullptr;
	CBitmap		m_bitmap;


	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILEDLG };
#endif
public:
	CFileDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFileDlg();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	void PictureChange(int IDB_NUM);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	BOOL sampleOrTool();
	void SetPen();
};
