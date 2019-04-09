
// ValidateUIView.h : interface of the CValidateUIView class
//

#pragma once


class CValidateUIView : public CView
{
protected: // create from serialization only
	CValidateUIView();
	DECLARE_DYNCREATE(CValidateUIView)

// Attributes
public:
	CValidateUIDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CValidateUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ValidateUIView.cpp
inline CValidateUIDoc* CValidateUIView::GetDocument() const
   { return reinterpret_cast<CValidateUIDoc*>(m_pDocument); }
#endif

