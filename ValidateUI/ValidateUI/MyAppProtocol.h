#pragma once
#include "stdafx.h"

///
//명령어 구조체의 코드 멤버 값
typedef enum COMMAND
{
	COMMAND_ERROR = 50,

	COMMAND_SND_SAMPLE = 100,
	COMMAND_SND_TOOL = 200,

	COMMAND_BEGIN_FILE = 250,
	COMMAND_END_FILE = 260,

	COMMAND_RUN_SAMPLE = 300,
	COMMAND_END_SAMPLE = 310,
	COMMAND_LOG_SAMPLE = 320,

	COMMAND_RUN_TOOL = 400,
	COMMAND_END_TOOL = 410,
	COMMAND_LOG_TOOL = 420,

	COMMAND_STOP = 500,
	COMMAND_READY = 600,
	COMMAND_HEALTH = 0
} COMMAND;

///
//기본헤더
typedef struct MYCOMMAND
{
	int nCode;
	int nSize;
	int nVersion;
} MYCOMMAND;

///
//에러
typedef struct RETURNMESSAGE
{
	int nErrorCode;	
	char szDesc[256];
} RETURNMESSAGE;

///
//FILEINFO
typedef struct FILEINFO
{
	TCHAR szFileName[_MAX_FNAME];
	DWORD dwFileSize;
} FILEINFO;

typedef struct MYLOG {
	int nCode;
	int nSize;
	int nVersion;
	CTime cNow;
} MYLOG;