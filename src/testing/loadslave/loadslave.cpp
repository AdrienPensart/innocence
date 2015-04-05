#include <common/ParseOptions.hpp>
#include <log/Log.hpp>
#include <audit/Audit.hpp>

int submain(int argc, char ** argv) {
	try {
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		// Chargement normal de la DLL
		HMODULE hModule = LoadLibrary("slave_dll.dll");
		//HMODULE hModule = LoadLibrary("isinjected.dll");
		Sleep(INFINITE);
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN