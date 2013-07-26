#include "Process.hpp"
#include "ProcessManager.hpp"

#include <cstdio>
#include <common/Log.hpp>
using namespace std;

namespace System
{
	Process::Process(const std::string& executable, const std::string& args, bool show)
	{
		LOG << "Creating process " + executable + " " + args;
		memset(&ExecuteInfo, 0, sizeof(ExecuteInfo));
		ExecuteInfo.cbSize       = sizeof(ExecuteInfo);
		ExecuteInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
		ExecuteInfo.hwnd         = 0;
		ExecuteInfo.lpVerb       = 0;
		ExecuteInfo.lpFile       = executable.c_str();
		ExecuteInfo.lpParameters = args.c_str();
		ExecuteInfo.lpDirectory  = 0;
		ExecuteInfo.nShow        = show ? SW_SHOW : SW_HIDE;
		ExecuteInfo.hInstApp     = 0;
	
		CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if(!ShellExecuteEx(&ExecuteInfo))
		{
			running = false;
			LOG << "ShellExecuteEx failed : " + to_string(GetLastError());
		}
		else
		{
			running = true;
			LOG << "ShellExecuteEx succeded : " + to_string(GetLastError());
		}
	}

	DWORD Process::getPid()
	{
		return GetProcessId(ExecuteInfo.hProcess);
	}

	DWORD Process::wait()
	{
		DWORD exitCode = EXIT_FAILURE;
		if(isRunning())
		{
			WaitForSingleObject(ExecuteInfo.hProcess,INFINITE);
			GetExitCodeProcess(ExecuteInfo.hProcess, &exitCode);
		}
		return exitCode;
	}

    void Process::kill()
    {
        ProcessManager::KillProcess(getPid());
    }

	bool Process::isRunning()
	{
		return running;
	}

	bool Process::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
	{
		TOKEN_PRIVILEGES tp;
		LUID luid;

		if ( !LookupPrivilegeValue(NULL, lpszPrivilege, &luid) )
		{
			LOG << "LookupPrivilegeValue failed : " + to_string(GetLastError());
			return false;
		}

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		if (bEnablePrivilege)
		{
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
		else
		{
			tp.Privileges[0].Attributes = 0;
		}

		if ( !AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL,(PDWORD) NULL) )
		{
			LOG << "AdjustTokenPrivileges failed : " + to_string(GetLastError());
			return false;
		}
		if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
		{
			LOG << "The token does not have the specified privilege";
			return false;
		}
		return true;
	}
} /* System */
