#pragma once

// CMySocket command target

class CMySocket : public CObject
{
	SOCKET m_hSocket;
	SOCKET m_cSocket;
public:
	CMySocket();
	virtual ~CMySocket()		{ 
		::WSACleanup();
		::DeleteCriticalSection(&m_cs);
	};
	
	void InitCS();

	void ErrorHandler(const char* pszMessage) const;
	BOOL mySocket();

	BOOL AddUser(SOCKET hClient);
	BOOL DeleteUser(SOCKET hClient);
	BOOL CloseAll();					//also thread close

	SOCKET Get_hSocket() const; 
	SOCKET Get_cSocket() const;

	BOOL SendCommandToAll(int nCommandCode, int Size);
	void SendCommandToOne(int nCommandCode, int Size, SOCKET cSocket);
	
	BOOL SendFileToAll(CString& cFilePath);
	
	BOOL closeFlagOn();
	BOOL getFlag() const;

protected:
private:
	CRITICAL_SECTION m_cs;
	CList<SOCKET, SOCKET> m_listClient;
	BOOL m_bCloseFlag;
};
