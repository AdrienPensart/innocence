#pragma once

#include <string>
#include <common/WindowsWrapper.hpp>

namespace System
{
	enum SystemVersion
	{
		OS_ERROR = -1,
		OS_WIN32_WINDOWS_NT_3_51 = 0,
		OS_WIN32_WINDOWS_NT_4_0,
		OS_WIN32_WINDOWS_95,
		OS_WIN32_WINDOWS_98,
		OS_WIN32_WINDOWS_Me,
		OS_WIN32_WINDOWS_2000,
		OS_WIN32_WINDOWS_XP,
		OS_WIN32_WINDOWS_SERVER_2003_FAMILY,
		OS_WIN32_WINDOWS_VISTA,
		OS_WIN32_WINDOWS_SEVEN,
		OS_UNKNOW
	};

	SystemVersion getSystemVersion();
	std::string getWindowsPath();
	std::string getSystemVersionString();
	std::string getComputerName();
	std::string getCompleteComputerName();
	bool shutdown();
	bool reboot();
	bool logout();
	bool hibernate();
	bool is64BitWindows();
} // System
