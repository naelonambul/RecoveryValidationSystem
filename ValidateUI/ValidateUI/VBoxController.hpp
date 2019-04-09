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
	VMSTART = 100,
	VMSTOP = 101,
	VMRECOVERYSNAP = 102,
	VMLIST = 103
} MYVM_COMMAND;

namespace VBox 
{

	class Controller 
	{
	public:
		CString MyProgramFilesPath;
		CString MyCommand;
		CString FileOutPut;

		CString cCurrentOS;

		map<string, string> mapOS;

		CPtrList outputCommand;
	public:
		void GetProgramFilesPath() {
			TCHAR path[MAX_PATH] = { 0 };
			ExpandEnvironmentStrings(_T("%ProgramW6432%"), path, sizeof(path));
			
			MyProgramFilesPath = _T("\"");
			MyProgramFilesPath += path;
			MyProgramFilesPath += _T("\\Oracle\\VirtualBox\\VBoxManage.exe");
			MyProgramFilesPath += _T("\"");
		}

		bool ExistFile() {
			const int existFile = _taccess(MyProgramFilesPath, 0x00);

			return (0 == existFile);
		}

		void makeTailCommand(int nOrder)
		{
			MyCommand = MyProgramFilesPath;

			switch (nOrder)
			{
			case	VMSTART:	MyCommand += " startvm ";
								MyCommand += cCurrentOS;
								//MyCommand += " --type headless";
								break;
			
			case	VMSTOP:		MyCommand += " controlvm ";
								MyCommand += cCurrentOS; 
								MyCommand += " poweroff "; 
								break;

			case	VMRECOVERYSNAP:	MyCommand += " snapshot ";
									MyCommand += cCurrentOS;
									MyCommand += " restore \"Recovery\" ";
									break;

			case	VMLIST		:	MyCommand += " list -l vms";
									break;
			}
		}


		void Run()
		{
			FILE* fp = RunShellCommand(MyCommand, READ_MODE);

			unsigned char buffer[1024 + 1] = { 0 };
			size_t count = 0;

			while (!feof(fp)) {
				count = fread(&buffer, 1, sizeof(buffer) - 1, fp);	
				buffer[count] = '\0';
				
				string str((char*)buffer);
				 
				string FoundName;
				string FoundOS;
				string FoundError;
				regexPipe(str, "^Name:\\s+(.*)\r\n", FoundName);
				regexPipe(str, "^Guest OS:\\s+(.*)\r\n", FoundOS);
				if(regexPipe(str, "error:(.*)\r\n", FoundError))				
				AfxMessageBox(CString(FoundError.c_str()));

				if(FoundName != "" && FoundOS != "")
					mapOS.insert(pair<string, string>(FoundName, FoundOS));
			}
			Sleep(1);
		}

		void SetCurrentOS(CString &nOSVersion)
		{
			cCurrentOS = _T("\"");
			cCurrentOS += nOSVersion;
			cCurrentOS += _T("\" ");
		}

		void exitVM() {
			map<string, string> ::iterator PrintIter;
			if (mapOS.empty() == FALSE)	{
				for (PrintIter = mapOS.begin();
					PrintIter != mapOS.end();
					PrintIter++) {
					wstring printBuffer = wstring(
						PrintIter->first.begin(), PrintIter->first.end());	{
						CString transbuffer(PrintIter->first.c_str());
						SetCurrentOS(transbuffer);
						makeTailCommand(VMSTOP);
						Run(); 
						Sleep(500);

						makeTailCommand(VMRECOVERYSNAP);
						Run();
						Sleep(500);
					}
				}
			}
		}
		 
		bool regexPipe(string& sTarget,const char* sentence, string& sFound)
		{
			regex regName(sentence);
			smatch m;
			if (regex_search(sTarget, m, regName)) {
				for (auto& buffer : m)
					sFound = buffer;
			}

			if (sFound == "")	return false;
			else				return true;
		}
};
}
  
 
#endif