#pragma once

#define BUILD 15

namespace Innocence
{
#ifdef PROJECT_MODULE
	static const char * MODULE = PROJECT_MODULE;
#else
	static const char * MODULE = "undefined project module";
#endif

	static const char * ID = "BUILD";
	static const char * DATE = "__DATE__";
	static const char * TIME = "__TIME__";
	static const char * TIMESTAMP = "__TIMESTAMP__";
}
