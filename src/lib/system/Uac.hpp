#pragma once

#include <string>
#include <common/Exception.hpp>

namespace System
{
	typedef Common::Exception UACException;
	bool isUacActivated();
	bool isAdministrator();
    std::string GetElevationType();
	bool RunAsAdministrator(const std::string& exe, const std::string& dir, bool show=true);
} // System
