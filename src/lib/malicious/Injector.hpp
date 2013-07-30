//!
//!     Kaleidoscalp, all rights reserved.
//!

#ifndef _INJECTOR_HPP_
#define _INJECTOR_HPP_

#include <system/Process.hpp>
#include <string>

namespace Malicious
{
	void DeleteMyself(const char * tempfilename, const char * cmd);
	bool inject(DWORD pid, std::string dll);
    bool SetDebugPrivileges();

} // Malicious

#endif // _INJECTOR_HPP_
