#pragma once

#include <map>
#include <string>
#include <common/WindowsWrapper.hpp>

namespace System
{
	namespace Process
	{
		typedef std::map<DWORD, std::string> Map;
		typedef std::pair<DWORD, std::string> ProcessEntry;

		ProcessEntry GetParentProcessEntry(DWORD pid);
		DWORD GetParentPid(DWORD pid = GetCurrentProcessId());
		DWORD GetPidFromName(const std::string& name);
		std::string GetAllRunningProcess();
		bool KillProcess(const std::string& name);
		bool KillProcess(DWORD pid);
		bool GetProcessList(Process::Map& mapProcs);

		class Launcher
		{
			public:

				Launcher(const std::string& executable, const std::string& args="", bool show=false);
				DWORD getPid();
				bool isRunning();
				DWORD wait();
				void kill();

			protected:

				bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
				//SHELLEXECUTEINFO ExecuteInfo;
				HANDLE process;
				DWORD pid;
				bool running;
		};

		class This
		{
			public:

				This();
				~This();
				DWORD getPid();
				DWORD getParentPid();
				std::string getPath();
				const std::string& getProgramPath();
				const std::string& getProgramName();
				const std::string& getProgramDir();
				int& getArgCount();
				const std::string getArg(unsigned int index);
				char ** getArgs();
				void killHierarchy();
				bool runAsAdmin();

			private:

				wchar_t ** argvw;
				char ** argv;
				int argc;
				std::string programPath;
				std::string programName;
				std::string programDir;
		};
	} // Process

} // System
