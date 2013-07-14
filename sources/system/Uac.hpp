#ifndef _UAC_HPP_
#define _UAC_HPP_

#include <string>

namespace System
{
	bool isUacActivated();
	bool isAdministrator();
    std::string GetElevationType();
	bool RunAsAdministrator(const std::string& exe, const std::string& dir, bool show);

} /* System */

#endif // _UAC_HPP_
