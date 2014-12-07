#pragma once

#include "Identity.hpp"
#include "Common.hpp"

namespace Common
{

#define BUILD 155

#ifdef PROJECT_MODULE
	static const char MODULE [] = PROJECT_MODULE;
#else
	static const char MODULE [] = "undefined project module";
#endif

	static Identity identity(MODULE, QUOTE(BUILD), __DATE__, __TIME__, __TIMESTAMP__);

} // Common
