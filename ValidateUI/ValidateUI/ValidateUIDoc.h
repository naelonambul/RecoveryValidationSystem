
// ValidateUIDoc.h : interface of the CValidateUIDoc class
//


#pragma once
#include <queue>
#include "resource.h"
using std::queue;


class CValidateUIDoc : public CDocument
{
protected: // create from serialization only
	CValidateUIDoc();
	DECLARE_DYNCREATE(CValidateUIDoc)

// Attributes
public:
	CPtrList Version07;
	CPtrList Version08;
	CPtrList Version10;

protected:
	CString pszSamplePath = _T("");
	CString pszToolPath = _T("");
	
	CString transBuffer = _T(" ");
// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CValidateUIDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	
public:
	void releasePtrList(CPtrList &nParam);
	void clearPtrList();

	void SetSamplePath(CString &cPath);
	void SetToolPath(CString  &cPath);

	CString& GetSamplePath();
	CString& GetToolPath();
	void CmdToCString(const int nCmd);

	BOOL exportCSV07();

	BOOL exportCSV08();
	BOOL exportCSV10();
};
