#include "stdafx.h"

#include "utility.hpp"
#include "DisplayView.h"

#include <tchar.h>
#include <shlobj.h>
#include <io.h>
#include <regex>
#include <map>

using std::regex;
using std::string;
using std::wstring;
using std::smatch;
using std::basic_regex;
using std::map;
using std::pair;
using std::regex_search;

#ifndef VBOX_CONTROLLER_HPP
#define VBOX_CONTROLLER_HPP

typedef enum MYVM_COMMAND {
	WMRUNNING = 0,
	WMREADY = 1,

	VMSTART = 100,
	VMSTOP = 101,
	VMRECOVERYSNAP = 102,
	VMLIST = 103
} MYVM_COMMAND;

namespace VBox {
class Controller 
{
public:
	int m_nRunning = WMREADY; //Ready = 0, Busy = 1
	int m_nRunMachine = 0;	//up to 3

	CString m_MyProgramFilesPath;
	CString m_MyCommand;

	CPtrList m_outputCommand;
	CString m_FileOutPut;

	CString m_CurrentOS;
	map<string, string> m_mapOS;

public:
	void GetProgramFilesPath();
	bool ExistFile();
	void makeTailCommand(int nOrder);
	void Run(int nOrder);
	bool regexPipe(string& sTarget, const char* sentence, string& sFound);
	void SetCurrentOS(CString &cOSVersion);
	void exitAllVm();
	void exitOneVm(int nOSversion);
};
}

#endif