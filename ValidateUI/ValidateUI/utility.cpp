#include "stdafx.h"

#include "utility.hpp"

FILE* RunShellCommand(const TCHAR* command, const int mode)
{
	BOOL success = FALSE;

	// 1. 파이프 생성
	// 1-1. 보안 속성 설정(파이프 핸들 상속 여부)
	SECURITY_ATTRIBUTES securityAttr = { 0 };
	securityAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	securityAttr.bInheritHandle = TRUE;
	securityAttr.lpSecurityDescriptor = NULL;

	/*=======================================================*
	*                                                        *
	*  부모 자식 프로세스간 연결되는 파이프 구조                *
	*                                                        *
	*  Parent                                         Child  *
	*  +------+                                    +------+  *
	*  |  hParentWriteToChild  ---> hChildReadFromParent  |  *
	*  |      |                                    |      |  *
	*  |  hParentReadFromChild <--- hChildWriteToParent   |  *
	*  +------+                                    +------+  *
	*                                                        *
	*========================================================*/

	// 1-2. 부모->자식 연결 파이프 생성
	HANDLE hParentWriteToChild = NULL;
	HANDLE hChildReadFromParent = NULL;
	success = CreatePipe(
		&hChildReadFromParent, &hParentWriteToChild, &securityAttr, 0);
	if (FALSE == success)
	{
		return NULL;
	}

	success = SetHandleInformation(
		hParentWriteToChild, HANDLE_FLAG_INHERIT, 0);
	if (FALSE == success)
	{
		return NULL;
	}

	// 1-3. 부모<-자식 연결 파이프 생성
	HANDLE hChildWriteToParent = NULL;
	HANDLE hParentReadFromChild = NULL;
	success = CreatePipe(&hParentReadFromChild, &hChildWriteToParent, &securityAttr, 0);
	if (FALSE == success)
	{
		return NULL;
	}

	success = SetHandleInformation(hParentReadFromChild, HANDLE_FLAG_INHERIT, 0);
	if (FALSE == success)
	{
		return NULL;
	}

	// 2. 자식 프로세스 생성
	// 2-1. 자식 프로세스의 표준 입력, 출력, 에러출력 핸들 설정
	STARTUPINFO startupInfo = { 0 };
		
	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.hStdError = hChildWriteToParent;
	startupInfo.hStdOutput = hChildWriteToParent;
	startupInfo.hStdInput = hChildReadFromParent;
	
	startupInfo.wShowWindow = SW_HIDE;
	startupInfo.dwFlags |= STARTF_USESTDHANDLES;
	startupInfo.dwFlags |= STARTF_USESHOWWINDOW;

	PROCESS_INFORMATION processInfo = { 0 };


	// 2-2. 자식 프로세스 생성
	success = CreateProcess(
		NULL,
		(LPTSTR)command,
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&startupInfo,
		&processInfo
	);
	CloseHandle(hChildWriteToParent);
	CloseHandle(hChildReadFromParent);

	if (FALSE == success)
	{
		return NULL;
	}


	// 3. 핸들 반환
	if (READ_MODE == mode)
	{
		CloseHandle(hParentWriteToChild);

		const int fd = _open_osfhandle((intptr_t)hParentReadFromChild, 0);
		return _fdopen(fd, "r");
	}
	else if (WRITE_MODE == mode)
	{
		CloseHandle(hParentReadFromChild);

		const int fd = _open_osfhandle((intptr_t)hParentWriteToChild, 0);
		return _fdopen(fd, "w");
	}

	CloseHandle(hParentWriteToChild);
	CloseHandle(hParentReadFromChild);

	return NULL;
}