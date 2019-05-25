
// ValidateUIDoc.cpp : implementation of the CValidateUIDoc class
//

#include "stdafx.h"
#include <locale.h> //kor
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ValidateUI.h"
#endif

#include "ValidateUIDoc.h"

#include <propkey.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using std::queue;
// CValidateUIDoc

IMPLEMENT_DYNCREATE(CValidateUIDoc, CDocument)

BEGIN_MESSAGE_MAP(CValidateUIDoc, CDocument)
END_MESSAGE_MAP()


// CValidateUIDoc construction/destruction

CValidateUIDoc::CValidateUIDoc()
{
	// TODO: add one-time construction code here

}

CValidateUIDoc::~CValidateUIDoc()
{
	clearPtrList();
}

BOOL CValidateUIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	theApp.SetValidateDoc(this);
	return TRUE;
}




// CValidateUIDoc serialization

void CValidateUIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CValidateUIDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CValidateUIDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CValidateUIDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CValidateUIDoc diagnostics

#ifdef _DEBUG
void CValidateUIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CValidateUIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CValidateUIDoc commands


void CValidateUIDoc::SetSamplePath(CString &cPath)
{
	m_pszSamplePath = cPath;
}


void CValidateUIDoc::SetToolPath(CString  &cPath)
{
	m_pszToolPath = cPath;
}


CString& CValidateUIDoc::GetSamplePath()
{
	return m_pszSamplePath;
}


CString& CValidateUIDoc::GetToolPath()
{
	return m_pszToolPath;
}



void CValidateUIDoc::clearPtrList()
{
	releasePtrList(m_Version07);
	releasePtrList(m_Version08);
	releasePtrList(m_Version10);
}

void CValidateUIDoc::releasePtrList(CPtrList &nParam)
{
	while (!nParam.IsEmpty())
	{
		MYLOG* pData = (MYLOG*)nParam.RemoveHead();
		delete pData;
		pData = nullptr;
	}
}


void CValidateUIDoc::CmdToCString(const int nCmd)
{
	switch (nCmd) {
	case COMMAND_HEALTH:		m_transBuffer.LoadString(IDS_STRING_HEALTH); break;
	case COMMAND_ERROR: 		m_transBuffer.LoadString(IDS_STRING_ERROR); break;
	case COMMAND_SND_SAMPLE:	m_transBuffer.LoadString(IDS_STRING_SND_SAMPLE); break;
	case COMMAND_SND_TOOL:		m_transBuffer.LoadString(IDS_STRING_SND_TOOL); break;
	case COMMAND_BEGIN_FILE:	m_transBuffer.LoadString(IDS_STRING_BEGIN_FILE); break;
	case COMMAND_END_FILE:		m_transBuffer.LoadString(IDS_STRING_END_FILE); break;
	case COMMAND_RUN_SAMPLE:	m_transBuffer.LoadString(IDS_STRING_RUN_SAMPLE); break;
	case COMMAND_END_SAMPLE:	m_transBuffer.LoadString(IDS_STRING_END_SAMPLE); break;
	case COMMAND_LOG_SAMPLE:	m_transBuffer.LoadString(IDS_STRING_LOG_SAMPLE); break;
	case COMMAND_RUN_TOOL:		m_transBuffer.LoadString(IDS_STRING_RUN_TOOL); break;
	case COMMAND_END_TOOL:		m_transBuffer.LoadString(IDS_STRING_END_TOOL); break;
	case COMMAND_LOG_TOOL:		m_transBuffer.LoadString(IDS_STRING_LOG_TOOL); break;
	case COMMAND_STOP:			m_transBuffer.LoadString(IDS_STRING_STOP); break;
	case COMMAND_READY:			m_transBuffer.LoadString(IDS_STRING_AGENTREADY); break;
	}
}

int CValidateUIDoc::exportCsvVersion(int nOsVersion)
{
	_wsetlocale(LC_ALL, L"kor");
	FILE* fp = nullptr;
	CPtrList* pPtrList = nullptr;
	switch (nOsVersion) {
	case 7:
		fopen_s(&fp, "Report07.csv", "w+");
		pPtrList = &m_Version07;
		break;
	case 8:
		fopen_s(&fp, "Report08.csv", "w+");
		pPtrList = &m_Version08;
		break;
	case 10:
		fopen_s(&fp, "Report10.csv", "w+");
		pPtrList = &m_Version10;
		break;
	}

	_ftprintf(fp, _T("시간, 윈도우 버전, 명령어, 비교후 같은 해시의 개수\n"));

	POSITION pos = pPtrList->GetHeadPosition();
	MYLOG* csvBuffer;
	CString logBuffer;
	
	int nFileCount = 0;
	int nInfestFile = 0;
	int nRecoveryFile = 0;

	while (pos != NULL)
	{
		csvBuffer = (MYLOG*)pPtrList->GetNext(pos);
		if (csvBuffer != NULL) {

			_ftprintf(fp, _T("%d-%d-%d %d:%d:%d,"),
				csvBuffer->cNow.GetYear(),
				csvBuffer->cNow.GetMonth(),
				csvBuffer->cNow.GetDay(),
				csvBuffer->cNow.GetHour(),
				csvBuffer->cNow.GetMinute(),
				csvBuffer->cNow.GetSecond());
			logBuffer.Format(_T("%d,"), csvBuffer->nVersion);
			CmdToCString(csvBuffer->nCode);
			logBuffer += m_transBuffer;

			if (csvBuffer->nCode == COMMAND_READY ||
				csvBuffer->nCode == COMMAND_LOG_SAMPLE ||
				csvBuffer->nCode == COMMAND_LOG_TOOL) {
				logBuffer.AppendFormat(_T(", %d\n"), csvBuffer->nSize);
			}
			else {
				logBuffer += _T("\n");
			}

			if (csvBuffer->nCode == COMMAND_READY)		{ nFileCount = csvBuffer->nSize; }
			if (csvBuffer->nCode == COMMAND_LOG_SAMPLE)	{ nInfestFile = csvBuffer->nSize; }
			if (csvBuffer->nCode == COMMAND_LOG_TOOL)	{ nRecoveryFile = csvBuffer->nSize;	}
			if (csvBuffer->nCode == COMMAND_STOP)		{
				_ftprintf(fp, _T("%s"), logBuffer.GetString());
				CString resultBuffer;
				resultBuffer.Format(_T("%02d%%"), (nFileCount - nInfestFile) / nRecoveryFile * 100);
				_ftprintf(fp, _T(",, 복구된 파일의 비율,"));
				_ftprintf(fp, _T("%s\n"), resultBuffer.GetString());
			}
			else { _ftprintf(fp, _T("%s"), logBuffer.GetString()); }
		
		}
	}
	_ftprintf(fp, _T("\n"));
	fclose(fp);

	return nOsVersion;
}