#pragma once
#include "stdafx.h"

#define BUFSIZE 1024
#define MD5LEN  16

#define WIN_MAJOR_MAX 10	//Windows 10
#define WIN_MAJOR_MIN 5		//Windows 2000 or XP

// Testheader
// C System
// C++ System
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <Wincrypt.h>
#include "atlstr.h"
#include <vector>
#include <map>
#include <string>
#include <io.h>
#include <time.h>
#include <stdio.h>
#include <versionhelpers.h>
#include <thread>
// Other Lib
#include <stdlib.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
// this Project header
#include "MyAppProtocol.h"

#include <tchar.h>
#include <shlobj.h>

using ::std::string;
using ::std::map;
using ::std::pair;
using ::std::iterator;
using ::std::cout;
using ::std::endl;

namespace MyData {

	enum g_Time {
		WaitExe = 60000,
		WaitWatch = 10000
	};

	class cDataStruct
	{
	//attributes
	public:
		DWORD m_dwPIdKill = 0;

		HANDLE SampleEvent = 0;
		HANDLE ToolEvent = 0;

		int m_nCountFile = 0;
		int m_nVersion = 0;
		
		int previousCommand = 0;
	protected:
		FILEINFO* m_pCurrentFile;
		FILEINFO* m_pSampleFile;
		FILEINFO* m_pToolFile;

		map<string, string> Sample_Before_Name_Hash;
		map<string, string> Sample_After_Name_Hash;
		map<string, string> Tool_After_Name_Hash;

	private:

	//func
	public:
		cDataStruct() {}
		~cDataStruct() {
			ReleaseFileInfo(); 
			ReleaseMap();
		}

	//attributes func
		void SetCurrentFileInfo(FILEINFO *pParam)	{
			delete m_pCurrentFile;
			m_pCurrentFile = pParam;
		}

		void SetSampleFileInfo(FILEINFO *pParam)	{
			m_pSampleFile = pParam;
		}

		void SetToolFileInfo(FILEINFO *pParam)		{
			m_pToolFile = pParam;
		}

		FILEINFO* GetSampleFileInfo() const			{
			return m_pSampleFile;
		}

		FILEINFO* GetToolFileInfo() const			{
			return m_pToolFile;
		}

		FILEINFO* GetCurrentFileInfo() const		{
			return m_pCurrentFile;
		}

		DWORD getPIdKill() const					{
			return m_dwPIdKill;
		}
		//map func
		map<string, string>& Sample_Before()			{
			return Sample_Before_Name_Hash;
		}

		map<string, string>& Sample_After()			{
			return Sample_After_Name_Hash;
		}

		map<string, string>& Tool_After()				{
			return Tool_After_Name_Hash;
		}

		int ReleaseMap()							{
			Sample_Before_Name_Hash.clear();
			Sample_After_Name_Hash.clear();
			Tool_After_Name_Hash.clear();

			return 0;
		}

		void DisPlayMap(map<string, string> mapParam)		{
			map<string, string> ::iterator PrintIter;
			if (mapParam.empty() == FALSE)
			{
				for (PrintIter = mapParam.begin();
					PrintIter != mapParam.end();
					PrintIter++) {
					string cName = PrintIter->first;
					string cHash = PrintIter->second;
					cout << "Display: " << cName << "		Hash:" << cHash << endl;
				}
			}
		}

		void ReleaseFileInfo()		{
			if(m_pCurrentFile != nullptr)
			delete m_pCurrentFile;
			if (m_pSampleFile != nullptr)
			delete m_pSampleFile;
			if (m_pToolFile != nullptr)
			delete m_pToolFile;
		}

		void createEvent()
		{
			SampleEvent = ::CreateEvent(
				NULL,	//디폴트 보안 속성 적용.
				FALSE,	//자동으로 상태 전환.
				FALSE,	//초기상태는 FALSE.
				NULL);	//이름 없음.

			ToolEvent = ::CreateEvent(
				NULL,	//디폴트 보안 속성 적용.
				FALSE,	//자동으로 상태 전환.
				FALSE,	//초기상태는 FALSE.
				NULL);	//이름 없음.
		}
};
///////////////////////////////////////////////////////////////////////
BOOL SendCommand(int nCommandCode, cDataStruct* agentData, SOCKET hSocket){
	MYCOMMAND Cmd;
	Cmd.nCode = nCommandCode;
	Cmd.nSize = 0;
	Cmd.m_nVersion = agentData->m_nVersion;

	if (agentData->m_nCountFile != 0) {
		Cmd.nSize = agentData->m_nCountFile;
		agentData->m_nCountFile = 0;
	}

	if (hSocket != NULL)
		::send(hSocket, reinterpret_cast<char*>(&Cmd), sizeof(Cmd), 0);
	
	memset(&Cmd, 0, sizeof(Cmd));
	return 0;
}
//BOOL SendError(const char* message) {
//	RETURNMESSAGE error;
//	error.szDesc
//}

/////////////////////////////////////////////////////////////////////
	string makehash(const char* filepath)	{
		DWORD dwStatus = 0;
		BOOL bResult = FALSE;
		HCRYPTPROV hProv = 0;
		HCRYPTHASH hHash = 0;
		HANDLE hFile = NULL;
		BYTE rgbFile[BUFSIZE];
		DWORD cbRead = 0;
		BYTE rgbHash[MD5LEN];
		DWORD cbHash = 0;
		CHAR rgbDigits[] = "0123456789abcdef";

		// Logic to check usage goes here.

		hFile = CreateFileA(filepath,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_SEQUENTIAL_SCAN,
			NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			dwStatus = GetLastError();
			printf("Error opening file %s\nError: %d\n", filepath,
				dwStatus);
		}

		// Get handle to the crypto provider
		if (!CryptAcquireContext(&hProv,
			NULL,
			NULL,
			PROV_RSA_FULL,
			CRYPT_VERIFYCONTEXT))
		{
			dwStatus = GetLastError();
			printf("CryptAcquireContext failed: %d\n", dwStatus);
			CloseHandle(hFile);
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
		{
			dwStatus = GetLastError();
			printf("CryptAcquireContext failed: %d\n", dwStatus);
			CloseHandle(hFile);
			CryptReleaseContext(hProv, 0);
		}

		while (bResult = ReadFile(hFile, rgbFile, BUFSIZE,
			&cbRead, NULL))
		{
			if (0 == cbRead)
			{
				break;
			}

			if (!CryptHashData(hHash, rgbFile, cbRead, 0))
			{
				dwStatus = GetLastError();
				printf("CryptHashData failed: %d\n", dwStatus);
				CryptReleaseContext(hProv, 0);
				CryptDestroyHash(hHash);
				CloseHandle(hFile);
			}
		}

		if (!bResult)
		{
			dwStatus = GetLastError();
			printf("ReadFile failed: %d\n", dwStatus);
			CryptReleaseContext(hProv, 0);
			CryptDestroyHash(hHash);
			CloseHandle(hFile);
		}

		cbHash = MD5LEN;
		char MDHash[33];				//need to Release
		memset(MDHash, 0, sizeof(MDHash));

		if (CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0))
		{
			for (DWORD i = 0; i < cbHash; i++)
			{
				MDHash[2 * i] = rgbDigits[rgbHash[i] >> 4];
				MDHash[2 * i + 1] = rgbDigits[rgbHash[i] & 0xf];
			}
			MDHash[cbHash * 2] = 0;
		}
		else
		{
			dwStatus = GetLastError();
			printf("CryptGetHashParam failed: %d\n", dwStatus);
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		CloseHandle(hFile);
		string MD5(MDHash);
		return MD5;
	}
	/////////////////////////////////////////////////////////////////////////
	int printList(map<string, string > &nParam)	{
		int nCountFile = 0;
		struct _finddata_t c_file;
		intptr_t hFile;

		char originalpath[MAX_PATH] = { 0 };
		SHGetSpecialFolderPathA(NULL, originalpath, CSIDL_MYDOCUMENTS, 0);
		char pathBuffer[MAX_PATH];

		strcpy_s(pathBuffer, sizeof(pathBuffer), originalpath);
		PathAppendA(pathBuffer, "*.*");
		// Find first .c file in current directory
		if ((hFile = _findfirst(pathBuffer, &c_file)) == -1L)
			printf("No *.c files in current directory!\n");
		else
		{
			printf("Listing of *.* files\n\n");
			printf("FILE          %cHASH\n", ' ');
			printf("----          %c----\n", ' ');
			do {
				char buffer[30];
				memset(pathBuffer, 0, sizeof(pathBuffer));
				strcpy_s(pathBuffer, sizeof(pathBuffer), originalpath);
				PathAppendA(pathBuffer, c_file.name);
				ctime_s(buffer, _countof(buffer), &c_file.time_write);

				string MD5HASH = makehash(pathBuffer);
				cout << c_file.name << "	" << MD5HASH << endl;
				string sName(c_file.name);
				if (MD5HASH != "" && sName != "desktop.ini") {
					nParam.insert(pair<string, string>(sName, MD5HASH));
					++nCountFile;
				}
			} while (_findnext(hFile, &c_file) == 0);
			_findclose(hFile);
		}
		return nCountFile;
	}
	//////////////////////////////////////////////////

	void KillProcess(DWORD dwPIdKill)
	{
		HANDLE hProcess = ::OpenProcess(
			PROCESS_TERMINATE,
			FALSE,
			dwPIdKill);
		if (hProcess != NULL)
		{
			::TerminateProcess(hProcess, (unsigned)-1);
			::CloseHandle(hProcess);
		}
	}

	void myFileRun(TCHAR* const filename)	{
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);

		if (::CreateProcess(filename,
			NULL,			//argv로 전달할 명령줄 문자열
			NULL,			//기본 보안속성
			NULL,			//기본 보안속성
			FALSE,			//핸들 상속하지 않음.
			0,				//기본값 사용
			NULL,			//부모 프로세스와 동일한 환경
			NULL,			//부모 프로세스와 동일한 경로
			&si,
			&pi) == TRUE)
		{
			::WaitForSingleObject(pi.hProcess, WaitExe);
			KillProcess(pi.dwProcessId);
			::CloseHandle(pi.hProcess);
			::CloseHandle(pi.hThread);
			_tprintf(_T("Running File is Closed"));
		}
	}

	/////////////////////////////////////////////////////
	void RefreshDirectory(LPTSTR lpDir, HANDLE )
	{
		_tprintf(TEXT("Directory (%s) changed.\n"), lpDir);
	}

	int WatchDirectory()	{

		TCHAR lpDir[MAX_PATH] = { 0 };
		SHGetSpecialFolderPathW(NULL, lpDir, CSIDL_MYDOCUMENTS, 0);

		DWORD dwWaitStatus = 0;
		HANDLE dwChangeHandles[2];
		TCHAR lpDrive[4];
		TCHAR lpFile[_MAX_FNAME];
		TCHAR lpExt[_MAX_EXT];

		_tsplitpath_s(lpDir, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

		lpDrive[2] = (TCHAR) '\\';
		lpDrive[3] = (TCHAR) '\0';

		dwChangeHandles[0] = FindFirstChangeNotification(
			lpDir,
			FALSE,
			FILE_NOTIFY_CHANGE_FILE_NAME);

		if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
		{
			printf("\n ERROR: FindFirstChangeNorification fuction failed.\n");
			ExitProcess(GetLastError());
		}
		int nCount = 0;
		while (dwWaitStatus != WAIT_TIMEOUT)
		{
			printf("Waiting for notification...");
			dwWaitStatus = WaitForSingleObject(dwChangeHandles[0], WaitWatch);

			switch (dwWaitStatus)
			{
			case WAIT_OBJECT_0:
				if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE)
				{
					printf("\n ERROR: FindNextChangeNorification function failed.\n");
					ExitProcess(GetLastError());
				}
				++nCount;	//count change
				break;

			case WAIT_TIMEOUT:
				printf("No changes in the timeout period.\n");
				break;

			default:
				printf("\n ERROR: Unhandled dwWaitStatus.\n");
				ExitProcess(GetLastError());
				break;
			}
		}

		return nCount;
	}
	////////////////////////////////////////////////////

	int CompareMap(map<string, string> &mAfter, map<string, string> &mBefore) {
		int nSame = 0;
		map<string, string> ::iterator mAfterIter;
		map<string, string> ::iterator mBeforeIter;

		for (mAfterIter = mAfter.begin(); mAfterIter != mAfter.end(); mAfterIter++) 
			for (mBeforeIter = mBefore.begin(); mBeforeIter != mBefore.end(); mBeforeIter++)
				if (mAfterIter->first == mBeforeIter->first)
					if(mAfterIter->second == mBeforeIter->second)
					++nSame;

		return nSame;
	}
	/////////////////////////////////////////////////////


	int Version_OS()
	{
		int dwMajor, dwMinor;
		DWORD dwTypeMask;

		DWORDLONG dwlConditionMask = 0;
		OSVERSIONINFOEX ovi;

		dwMajor = 0;
		dwMinor = 0;

		for (int mjr = WIN_MAJOR_MAX; mjr >= WIN_MAJOR_MIN; mjr--) {
				for (int mnr = 0; mnr <= 3; mnr++) {
				memset(&ovi, 0, sizeof(OSVERSIONINFOEX));
				ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

				ovi.dwMajorVersion = mjr;
				ovi.dwMinorVersion = mnr;

				dwlConditionMask = 0;

				VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
				VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

				dwTypeMask = VER_MAJORVERSION | VER_MINORVERSION;

				if (TRUE == VerifyVersionInfo(&ovi, dwTypeMask, dwlConditionMask)) {
					dwMajor = mjr;
					dwMinor = mnr;
					break;
				}
			}
		}
		if (dwMajor == 6 && dwMinor == 1)		return 7;
		else if (dwMajor == 6 && dwMinor == 3)		return 8;
		else										return 10;
	}

}
