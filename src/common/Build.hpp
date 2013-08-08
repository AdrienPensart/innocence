#pragma once

#include "Identity.hpp"

namespace Common
{

#define BUILD 54

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef PROJECT_MODULE
	static const char MODULE [] = PROJECT_MODULE;
#else
	static const char MODULE [] = "undefined project module";
#endif

	static Identity identity(MODULE, QUOTE(BUILD), __DATE__, __TIME__, __TIMESTAMP__);

} // Common
