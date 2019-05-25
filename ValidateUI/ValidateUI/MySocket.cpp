// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "ValidateUI.h"
#include "MySocket.h"

#pragma warning(disable:4996) 


// CMySocket
typedef enum PORT {
	SERVERPORT = 25000
}PORT;

CMySocket::CMySocket()
{

}

void CMySocket::ErrorHandler(const char* pszMessage) const
{
	AfxMessageBox(CString(pszMessage));
	int a = 0;
	
	if((a = WSAGetLastError()) != 0)
	AfxMessageBox(a);

	::WSACleanup();
}

BOOL CMySocket::mySocket()
{
	m_hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		ErrorHandler("mySocket Error");
		return FALSE;
	}

	SOCKADDR_IN svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(SERVERPORT);
	svraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	if (::bind(m_hSocket,
		(SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		ErrorHandler("mybind Error");
		return FALSE;
	}

	if (::listen(m_hSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		ErrorHandler("myListen Error");
		return FALSE;
	}

	SOCKADDR_IN clientaddr = { 0 };
	int nAddrLen = sizeof(clientaddr);
	DWORD dwThreadID = 0;

	while ((m_cSocket = ::accept(m_hSocket,
		(SOCKADDR*)&clientaddr, &nAddrLen)) != INVALID_SOCKET)
	{
		if (AddUser(m_cSocket) == FALSE)
		{
			ErrorHandler("ERROR: Cannot connect client more.");
			break;
		}
	CWinThread *pAcceptThread = AfxBeginThread(CValidateUIApp::ThreadCommand, this);
	
	}

	return TRUE;
}

BOOL CMySocket::AddUser(SOCKET hClient)
{
	::EnterCriticalSection(&m_cs);	
										
	m_listClient.AddTail(hClient);
	::LeaveCriticalSection(&m_cs);		

	return TRUE;
}


BOOL CMySocket::DeleteUser(SOCKET hClient)
{
	::EnterCriticalSection(&m_cs);

	POSITION pos = m_listClient.Find(hClient);
	if (pos != NULL)
		m_listClient.RemoveAt(pos);

	::LeaveCriticalSection(&m_cs);
	::closesocket(hClient);

	return 0;
}


BOOL CMySocket::CloseAll()
{
	::shutdown(m_hSocket, SD_BOTH);

	SOCKET sClient = NULL;
	POSITION pos = m_listClient.GetHeadPosition();

	::EnterCriticalSection(&m_cs);
	while(pos != NULL)
	{
		sClient = m_listClient.GetNext(pos);
		::closesocket(sClient);
	}
	::LeaveCriticalSection(&m_cs);
	::Sleep(100);
	::closesocket(m_hSocket);

	::WSACleanup();
	return 0;
}


SOCKET CMySocket::Get_hSocket() const
{
	if (!m_hSocket) { ErrorHandler("Invalid m_hSocket"); return NULL; }

	return m_hSocket;
}

SOCKET CMySocket::Get_cSocket() const
{
	if (!m_cSocket) { ErrorHandler("Invalid m_cSocket"); return NULL; }

	return m_cSocket;
}

void CMySocket::InitCS()
{
	::InitializeCriticalSection(&m_cs);
}

BOOL CMySocket::SendCommandToAll(int nCommandCode, int Size)
{
	POSITION pos = m_listClient.GetHeadPosition();
	SOCKET sClient = NULL;

	MYCOMMAND Cmd;
	Cmd.nCode = nCommandCode;
	Cmd.nSize = 0;
	Cmd.nVersion = 0;

	::EnterCriticalSection(&m_cs);
	while (pos != NULL)
	{
		sClient = m_listClient.GetNext(pos);
		if (sClient != NULL)
			::send(sClient, reinterpret_cast<char*>(&Cmd), sizeof(Cmd), 0);
	}
	::LeaveCriticalSection(&m_cs);
	return 0;
}


void CMySocket::SendCommandToOne(int nCommandCode, int Size, SOCKET cSocket)
{
	MYCOMMAND Cmd;
	Cmd.nCode = nCommandCode;
	Cmd.nSize = 0;
	Cmd.nVersion = 0;

	::send(cSocket, reinterpret_cast<char*>(&Cmd), sizeof(Cmd), 0);
}



BOOL CMySocket::SendFileToAll(CString& cFilePath)
{
		POSITION pos = m_listClient.GetHeadPosition();
		SOCKET sClient = NULL;

		TCHAR tcFile[_MAX_FNAME];
		_tcscpy_s(tcFile, cFilePath);
		TCHAR *tcFileName;
		tcFileName = PathFindFileName(tcFile);

		while (pos != NULL)
		{
			::EnterCriticalSection(&m_cs);

			HANDLE SendFile = ::CreateFile(cFilePath,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_SEQUENTIAL_SCAN,
				NULL);
			if (SendFile == INVALID_HANDLE_VALUE)
				ErrorHandler("전송할 파일을 개방할 수 없습니다.");

			FILEINFO myData;
			_tcscpy_s(myData.szFileName, tcFileName);
			myData.dwFileSize = ::GetFileSize(SendFile, NULL);

			TRANSMIT_FILE_BUFFERS tfsb = { 0 };
			tfsb.Head = &myData;
			tfsb.HeadLength = sizeof(myData);

			sClient = m_listClient.GetNext(pos);
			if (sClient != NULL) {
				if(::TransmitFile(
					sClient,	
					SendFile,	
					0,			
					65535,		
					NULL,		
					&tfsb,		
					0			
				) == FALSE)
					ErrorHandler("파일을 전송할 수 없습니다.");
			::CloseHandle(SendFile);

			::LeaveCriticalSection(&m_cs);
			}
		}
	return 0;
}



BOOL CMySocket::closeFlagOn()
{
	if (m_bCloseFlag == FALSE)
		m_bCloseFlag = TRUE;
	return m_bCloseFlag;
}


BOOL CMySocket::getFlag() const
{
	return m_bCloseFlag;
}
