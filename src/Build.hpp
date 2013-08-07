#pragma once

#include <Identity.hpp>

namespace Innocence
{

#define BUILD 41

#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef PROJECT_MODULE
	static const char MODULE [] = PROJECT_MODULE;
#else
	static const char MODULE [] = "undefined project module";
#endif

	static Identity identity(MODULE, QUOTE(BUILD), __DATE__, __TIME__, __TIMESTAMP__);

} // Innocence
