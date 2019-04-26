// InternalAgent.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERIP "192.168.56.1"

#include "InternalAgent.h"

using ::std::string;
using ::std::vector;
using ::MyData::cDataStruct;

using ::MyData::makehash;
using ::MyData::myFileRun;
using ::MyData::printList;
using ::MyData::WatchDirectory;
using ::MyData::CompareMap;
using ::MyData::SendCommand;
using ::MyData::Version_OS;
using ::MyData::SendError;

typedef int(*COMD_FUNC)(cDataStruct*, SOCKET);

enum g_IPINFO {
	DEFAULT_BUFLEN = 512,
	ContolPort = 25000,
};

int p_ERRORfuc		(cDataStruct *agentData, SOCKET hSocket);
int p_SND_SAMPLEfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_SND_TOOLfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_BEGIN_FILEfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_END_FILEfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_RUN_SAMPLEfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_END_SAMPLEfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_RUN_TOOLfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_END_TOOLfuc	(cDataStruct *agentData, SOCKET hSocket);
int p_HEALTHfuc		(cDataStruct *agentData, SOCKET hSocket);
int p_STOPfuc		(cDataStruct *agentData, SOCKET hSocket);

DWORD WINAPI MySampleThread(LPVOID lpParam);
DWORD WINAPI MyToolThread(LPVOID lpParam); 

int main()
{
	int nOSVersion = Version_OS();
	printf("OSVersion : %d \n", nOSVersion);

	cDataStruct myData;
	MYCOMMAND cBuffer;

	myData.m_nVersion = nOSVersion;
	myData.createEvent();

	//Save Hash
	myData.m_nCountFile = printList(myData.Sample_Before());
	myData.DisPlayMap(myData.Sample_Before());
	puts("");

	WSADATA wsa = { 0 };
	if (::WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		puts("ERROR: Unable to initial Winsock\n");
		return 0;
	}

	SOCKET hSocket = ::socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		puts("ERROR: Unable to create socket\n");
		return 0;
	}
	struct timeval tv = { 0 };
	tv.tv_sec = 5000;
	setsockopt(hSocket, SOL_SOCKET, SO_RCVTIMEO, (char* )&tv, sizeof(struct timeval));

	//포트 바인딩 및 연결
	SOCKADDR_IN svraddr = { 0 };
	svraddr.sin_family = AF_INET;
	svraddr.sin_port = htons(25000);
	svraddr.sin_addr.S_un.S_addr = inet_addr(SERVERIP);
	if (::connect(hSocket,
		(SOCKADDR*)&svraddr, sizeof(svraddr)) == SOCKET_ERROR)
	{
		puts("ERROR: Unable to Connect to server.\n");
		return 0;
	}
	
	COMD_FUNC pAgentFuc[600]		= { 0 };
	pAgentFuc[COMMAND_ERROR]		= p_ERRORfuc;
	pAgentFuc[COMMAND_SND_SAMPLE]	= p_SND_SAMPLEfuc;
	pAgentFuc[COMMAND_SND_TOOL]		= p_SND_TOOLfuc;
	
	pAgentFuc[COMMAND_BEGIN_FILE]	= p_BEGIN_FILEfuc;
	pAgentFuc[COMMAND_END_FILE]		= p_END_FILEfuc;
	
	pAgentFuc[COMMAND_RUN_SAMPLE]	= p_RUN_SAMPLEfuc;
	pAgentFuc[COMMAND_RUN_TOOL]		= p_RUN_TOOLfuc;
	pAgentFuc[COMMAND_HEALTH]		= p_HEALTHfuc;
	pAgentFuc[COMMAND_STOP]			= p_STOPfuc;

	//Ready
	SendCommand(COMMAND_READY, &myData, hSocket);

	while (1) {
		cBuffer = { 0 };
		::recv(hSocket, (char*)&cBuffer, sizeof(cBuffer), 0);

		if (cBuffer.nCode == 0 ||
			cBuffer.nCode - myData.previousCommand == 100)
			pAgentFuc[cBuffer.nCode]((cDataStruct*)&myData, hSocket);
		else {
			SendError(_T("명령 순서가 맞지 않습니다."), &myData, hSocket);
			break;
		}

		if(cBuffer.nCode != 0)
			printf("Control Message Receive: %d\n", cBuffer.nCode);
		
		memset(&cBuffer, 0, sizeof(cBuffer));
	}

    return 0;
}

int p_ERRORfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("ERROR");
	SendCommand(COMMAND_ERROR, agentData,hSocket);
	return 1;
}

int p_SND_SAMPLEfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_SND_SAMPLE");
	//Send Command
	SendCommand(COMMAND_SND_SAMPLE, agentData, hSocket);
	agentData->previousCommand = COMMAND_SND_SAMPLE;
	//Receive File info
	FILEINFO sampleData = { 0 };
	if (::recv(hSocket, (char*)&sampleData, sizeof(sampleData), 0) < sizeof(sampleData))
		SendError(_T("샘플 정보를 받지 못했습니다."), agentData, hSocket);

	FILEINFO* pData = new FILEINFO(sampleData);
	FILEINFO* pDataCopy = new FILEINFO(sampleData);
	
	agentData->SetCurrentFileInfo(pData);
	agentData->SetSampleFileInfo(pDataCopy);

	p_BEGIN_FILEfuc(agentData, hSocket);
	return 1;
}

int p_SND_TOOLfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_SND_TOOL");
	//Send Command
	SendCommand(COMMAND_SND_TOOL, agentData, hSocket);
	agentData->previousCommand = COMMAND_SND_TOOL;
	//Receive File info
	FILEINFO toolData = { 0 };
	if (::recv(hSocket, (char*)&toolData, sizeof(toolData), 0) <sizeof(toolData))
		SendError(_T("복구 도구 정보를 받지 못했습니다."), agentData, hSocket);

	FILEINFO* pData = new FILEINFO(toolData);
	FILEINFO* pDataCopy = new FILEINFO(toolData);

	agentData->SetCurrentFileInfo(pData);
	agentData->SetToolFileInfo(pDataCopy);

	p_BEGIN_FILEfuc(agentData, hSocket);
	return 1;
}

int p_BEGIN_FILEfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_BEGIN_FILE");
	//Send Command
	SendCommand(COMMAND_BEGIN_FILE, agentData, hSocket);
	//Open File
	puts("*** Start Recive File. ***");
	HANDLE hFile = ::CreateFile(
		agentData->GetCurrentFileInfo()->szFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,	// Mode
		0,
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		SendError(_T("전송된 파일을 열지 못했습니다."), agentData, hSocket);

	//Receive
	char byBuffer[65535] = {0};
	int nRecv = 0;
	DWORD dwTotalRecv = 0, dwRead = 0;
	while (dwTotalRecv < agentData->GetCurrentFileInfo()->dwFileSize)
	{
		if ((nRecv = ::recv(hSocket, byBuffer, 65535, 0)) > 0)
		{
			dwTotalRecv += nRecv;
			::WriteFile(hFile, byBuffer, nRecv, &dwRead, NULL);
			printf("Receive: %d/%d\n", dwTotalRecv, agentData->GetCurrentFileInfo()->dwFileSize);
			fflush(stdout);
		}
		else
		{
			SendError(_T("전송이 중단됬습니다."), agentData, hSocket);
			break;
		}
	}

	::CloseHandle(hFile);

	p_END_FILEfuc(agentData, hSocket);
	return 1;
}
int p_END_FILEfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_END_FILE");
	SendCommand(COMMAND_END_FILE, agentData,hSocket);

	return 1;
}
int p_RUN_SAMPLEfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_RUN_SAMPLE");
	//Send Command
	SendCommand(COMMAND_RUN_SAMPLE, agentData, hSocket);
	agentData->previousCommand = COMMAND_RUN_SAMPLE;

	// Sample Run
	HANDLE hSamplethread = ::CreateThread(
		NULL,
		0,
		MySampleThread,
		(LPVOID)agentData,
		0,
		NULL);	

	if(hSamplethread == NULL)			
		SendError(_T("샘플을 실행하지 못했습니다."), agentData, hSocket);
	else {
		::SetEvent(agentData->SampleEvent);
		printf("Detect File Change: %d\n",WatchDirectory());
	}
	//p_END_SAMPLE call
	p_END_SAMPLEfuc(agentData, hSocket);
	return 1;
}
int p_END_SAMPLEfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_END_SAMPLE");

	//SendCommand
	SendCommand(COMMAND_END_SAMPLE, agentData,hSocket);

	//Print Map elements
	printList(agentData->Sample_After());
	puts("");
	//Print
	agentData->DisPlayMap(agentData->Sample_After());
	puts("");
	//Compare Hash
	agentData->m_nCountFile = agentData->m_nCountFile - CompareMap(agentData->Sample_After(), agentData->Sample_Before());
	//Send Log
	printf("Different File :%d\n", agentData->m_nCountFile);
	SendCommand(COMMAND_LOG_SAMPLE, agentData, hSocket);

	return 1;
}
int p_RUN_TOOLfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_RUN_TOOL");
	//SendCommand
	SendCommand(COMMAND_RUN_TOOL, agentData, hSocket);
	agentData->previousCommand = COMMAND_RUN_TOOL;

	int m_nCountRecovery = 0;
	// Tool Run
	HANDLE hToolthread = ::CreateThread(
		NULL,
		0,
		MyToolThread,
		(LPVOID)agentData,
		0,
		NULL);

	if (hToolthread == NULL) 
		SendError(_T("복구도구를 실행하지 못했습니다."), agentData, hSocket);
	else {
		::SetEvent(agentData->ToolEvent);
		printf("Detect File Change: %d\n", WatchDirectory()); 
	}
	//END_TOOLfuc call
	p_END_TOOLfuc(agentData, hSocket);
	return 1;
}
int p_END_TOOLfuc(cDataStruct *agentData, SOCKET hSocket)
{
	puts("_END_TOOL");

	//SendCommand
	SendCommand(COMMAND_END_TOOL, agentData, hSocket);

	//Save Hash
	printList(agentData->Tool_After());
	puts("");

	//Print Map elements
	agentData->DisPlayMap(agentData->Tool_After());
	puts("");

	//Compare Hash
	agentData->m_nCountFile = CompareMap(agentData->Tool_After(), agentData->Sample_Before());

	//Send Recovery Log
	printf("Same File :%d\n", agentData->m_nCountFile);
	SendCommand(COMMAND_LOG_TOOL, agentData, hSocket);
	return 1;
}
int p_STOPfuc(cDataStruct *agentData, SOCKET hSocket)
{
	SendCommand(COMMAND_STOP, agentData, hSocket);
	::closesocket(hSocket);
	::Sleep(100);	//wait for exit
	::WSACleanup();
	return 1;
}

int p_HEALTHfuc(cDataStruct *agentData, SOCKET hSocket)
{
	//Health_check
	printf("_HEALTH");
	SendCommand(COMMAND_HEALTH, agentData, hSocket);

	return 1;
}

DWORD WINAPI MySampleThread(LPVOID lpParam) 
{
	cDataStruct *cSampleData = (cDataStruct*)lpParam;
	
	::WaitForSingleObject(cSampleData->SampleEvent, INFINITE);
	printf("MySampleThread\n");
	myFileRun(cSampleData->GetSampleFileInfo()->szFileName);

	return 0;
}

DWORD WINAPI MyToolThread(LPVOID lpParam) 
{
	cDataStruct* cToolData = (cDataStruct*)lpParam;

	::WaitForSingleObject(cToolData->ToolEvent, INFINITE);
	printf("MyToolThread\n");
	myFileRun(cToolData->GetToolFileInfo()->szFileName);

	return 0;
}