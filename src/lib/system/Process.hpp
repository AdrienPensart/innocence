#ifndef _PROCESS_
#define _PROCESS_

#include <string>
#include <windows.h>

namespace System
{
	/* Abstraction du processus sous Windows */
	class Process
	{
		public:

			Process(const std::string& executable, const std::string& args="", bool show=false);
			DWORD getPid();
			bool isRunning();
			DWORD wait();
            void kill();

		protected:

			bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
			SHELLEXECUTEINFO ExecuteInfo;
			bool running;
	};

} /* System */

#endif // _PROCESS_
