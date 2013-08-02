#pragma once

#include <string>
#include <exception>

namespace System
{
	typedef std::exception UACException;
	bool isUacActivated();
	bool isAdministrator();
    std::string GetElevationType();
	bool RunAsAdministrator(const std::string& exe, const std::string& dir, bool show=true);
} // System
