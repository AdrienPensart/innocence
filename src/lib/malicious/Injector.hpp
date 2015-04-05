#pragma once

#include <system/Process.hpp>
#include <common/Exception.hpp>
#include <string>

namespace Malicious {
	typedef Common::Exception InjectionError;

	void DeleteMyself(const char * tempfilename, const char * cmd);
	void inject(DWORD pid, std::string dll);
	//void SetDebugPrivileges();

} // Malicious
