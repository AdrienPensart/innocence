#ifndef _UAC_
#define _UAC_

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

#endif // _UAC_
