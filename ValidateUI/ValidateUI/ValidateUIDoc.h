
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
	CPtrList m_Version07;
	CPtrList m_Version08;
	CPtrList m_Version10;

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
	void SetSamplePath(CString &cPath);
	void SetToolPath(CString  &cPath);

	CString& GetSamplePath();
	CString& GetToolPath();

	int exportCsvVersion(int nOsVersion);
protected:
private:
	void CmdToCString(const int nCmd);
	void releasePtrList(CPtrList &nParam);
	void clearPtrList();
};
