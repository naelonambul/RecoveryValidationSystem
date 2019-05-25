#include "stdafx.h"




#include "VBoxController.hpp"

using namespace ::VBox;

void Controller::GetProgramFilesPath() {
	TCHAR path[MAX_PATH] = { 0 };
	ExpandEnvironmentStrings(_T("%ProgramW6432%"), path, sizeof(path));

	m_MyProgramFilesPath = _T("\"");
	m_MyProgramFilesPath += path;
	m_MyProgramFilesPath += _T("\\Oracle\\VirtualBox\\VBoxManage.exe");
	m_MyProgramFilesPath += _T("\"");
}

bool Controller::ExistFile() {
	const int existFile = _taccess(m_MyProgramFilesPath, 0x00);

	return (0 == existFile);
}

void Controller::makeTailCommand(int nOrder) {
	m_MyCommand = m_MyProgramFilesPath;

	switch (nOrder) {
	case	VMSTART:	m_MyCommand += " startvm ";
		m_MyCommand += m_CurrentOS;
		m_MyCommand += " --type headless";
		break;

	case	VMSTOP:		m_MyCommand += " controlvm ";
		m_MyCommand += m_CurrentOS;
		m_MyCommand += " poweroff ";
		break;

	case	VMRECOVERYSNAP:	m_MyCommand += " snapshot ";
		m_MyCommand += m_CurrentOS;
		m_MyCommand += " restore \"Recovery\" ";
		break;

	case	VMLIST:	m_MyCommand += " list -l vms";
		break;
	}
}

void Controller::Run(int nOrder) {
	m_nRunning = WMRUNNING;
	makeTailCommand(nOrder);
	FILE* fp = RunShellCommand(m_MyCommand, READ_MODE);

	unsigned char buffer[1024 + 1] = { 0 };
	size_t count = 0;

	while (!feof(fp)) {
		count = fread(&buffer, 1, sizeof(buffer) - 1, fp);
		buffer[count] = '\0';

		string str((char*)buffer);
		string ReturnPipe;

		//parsing error
		if (nOrder != VMSTOP) {
			if (regexPipe(str, "^error:\\s+(.*)\r\n", ReturnPipe))
				AfxMessageBox(_T("ERROR"));
		}

		if (nOrder == VMRECOVERYSNAP) {
			if (strcmp((char*)buffer, "100%"))
				if (m_nRunMachine > 0) --m_nRunMachine;
		}

		if (nOrder == VMSTART) {
			if (strcmp((char*)buffer, "successfully started"))
				if (m_nRunMachine < 3) ++m_nRunMachine;
		}
		//parsing vm list
		if (nOrder == VMLIST) {
			string FoundName;
			string FoundOS;
			regexPipe(str, "^Name:\\s+(.*)\r\n", FoundName);
			regexPipe(str, "^Guest OS:\\s+(.*)\r\n", FoundOS);
			if (FoundName != "" && FoundOS != "")
				m_mapOS.insert(pair<string, string>(FoundName, FoundOS));
		}
		memset(buffer, 0, sizeof(buffer));
	}
	m_nRunning = WMREADY;
}

bool Controller::regexPipe(string& sTarget, const char* sentence, string& sFound) {
	regex regName(sentence);
	smatch m;
	if (regex_search(sTarget, m, regName)) {
		for (auto& buffer : m)
			sFound = buffer;
	}

	if (sFound == "")	return false;
	else				return true;
}

void Controller::SetCurrentOS(CString &cOSVersion) {
	m_CurrentOS = _T("\"");
	m_CurrentOS += cOSVersion;
	m_CurrentOS += _T("\" ");
}

void Controller::exitAllVm() {
	map<string, string> ::iterator PrintIter;
	if (m_mapOS.empty() == FALSE) {
		for (PrintIter = m_mapOS.begin(); PrintIter != m_mapOS.end(); PrintIter++) {
			wstring printBuffer = wstring(PrintIter->first.begin(), PrintIter->first.end());
			CString transbuffer(PrintIter->first.c_str());
			SetCurrentOS(transbuffer);
			Run(VMSTOP);
			Sleep(300);

			Run(VMRECOVERYSNAP);
			Sleep(100);
		}
	}
}

void Controller::exitOneVm(int nOSversion)
{
	CString cOSVersion;

	switch (nOSversion) {
	case 7: cOSVersion = _T("IE11 - Win7");  break;
	case 8:	cOSVersion = _T("IE11 - Win81"); break;
	case 10: cOSVersion = _T("MSEdge - Win10"); break;
	}

	map<string, string> ::iterator PrintIter;
	if (m_mapOS.empty() == FALSE) {
		for (PrintIter = m_mapOS.begin(); PrintIter != m_mapOS.end(); PrintIter++) {
			wstring printBuffer = wstring(PrintIter->first.begin(), PrintIter->first.end());
			CString transbuffer(PrintIter->first.c_str());
			if (cOSVersion == transbuffer) {
				SetCurrentOS(transbuffer);
				Run(VMSTOP);
				Sleep(300);

				Run(VMRECOVERYSNAP);
				Sleep(100);
			}
		}
	}
}