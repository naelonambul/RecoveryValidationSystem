#pragma once

// CMySocket command target

class CMySocket : public CObject
{
	SOCKET m_hSocket;
	SOCKET m_cSocket;


public:
	CList<SOCKET, SOCKET> m_listClient;

	CRITICAL_SECTION m_cs;
public:
	CMySocket();
	virtual ~CMySocket() { ::WSACleanup(); };
	
	void InitCS();

	void ErrorHandler(const char* pszMessage);
	BOOL mySocket();

	BOOL AddUser(SOCKET hClient);
	BOOL CloseAll();

	SOCKET Get_hSocket(); 
	SOCKET Get_cSocket();
	BOOL SendCommandToAll(int nCommandCode, int Size);
	BOOL SendFileToAll(CString& cFilePath);

	HANDLE fileEvent;
	void SendCommandToOne(int nCommandCode, int Size, SOCKET cSocket);
};
