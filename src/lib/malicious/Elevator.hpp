#pragma once

#include <string>
#include <common/WindowsWrapper.hpp>

namespace Malicious
{
	static const char * ELEVATOR_PROCESS_NAME = "explorer.exe";
	static const char * ELEVATOR_DLL_NAME = "privilege.dll";
	static const char * ELEVATOR_EXE_NAME = "privilege.exe";
	DWORD elevate(std::string programPath);
} // Malicious
