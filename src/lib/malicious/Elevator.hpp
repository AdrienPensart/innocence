#ifndef _ELEVATOR_
#define _ELEVATOR_

#include <string>
#include <Windows.h>

namespace Malicious
{
	static const char * ELEVATOR_PROCESS_NAME = "explorer.exe";
	static const char * ELEVATOR_DLL_NAME = "privilege.dll";
	static const char * ELEVATOR_EXE_NAME = "privilege.exe";
	DWORD elevate(std::string programPath);
} // Malicious

#endif // _ELEVATOR_
