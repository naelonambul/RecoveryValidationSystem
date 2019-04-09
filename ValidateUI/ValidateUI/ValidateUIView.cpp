
// ValidateUIView.cpp : implementation of the CValidateUIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ValidateUI.h"
#endif

#include "ValidateUIDoc.h"
#include "ValidateUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CValidateUIView

IMPLEMENT_DYNCREATE(CValidateUIView, CView)

BEGIN_MESSAGE_MAP(CValidateUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CValidateUIView construction/destruction

CValidateUIView::CValidateUIView()
{
	// TODO: add construction code here

}

CValidateUIView::~CValidateUIView()
{
}

BOOL CValidateUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CValidateUIView drawing

void CValidateUIView::OnDraw(CDC* /*pDC*/)
{
	CValidateUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CValidateUIView printing

BOOL CValidateUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CValidateUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CValidateUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CValidateUIView diagnostics

#ifdef _DEBUG
void CValidateUIView::AssertValid() const
{
	CView::AssertValid();
}

void CValidateUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CValidateUIDoc* CValidateUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CValidateUIDoc)));
	return (CValidateUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CValidateUIView message handlers
