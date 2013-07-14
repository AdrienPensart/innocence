#ifndef _PROCESS_MANAGER_
#define _PROCESS_MANAGER_

#include <string>
#include <map>
#include <windows.h>

namespace System
{
	typedef std::map<DWORD, std::string> ProcessesMap;

    class ProcessManager
    {
        public:

            static DWORD GetPidFromName(const char * szProcessName);
            static std::string GetAllRunningProcess();
            static std::string GetCurrentProcessName();
            static bool KillProcess(std::string name);
            static bool KillProcess(DWORD pid);
    };

	bool GetProcessList(ProcessesMap& mapProcs);

} /* System */

#endif // _PROCESS_MANAGER_
