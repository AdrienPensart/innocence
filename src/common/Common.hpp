#pragma once

#ifdef WIN32
	#include <system/Process.hpp>
#endif

// hack to uniform subsystems in windows
#ifdef INNOCENCE_DEBUG
#define INNOCENCE_MAIN \
int main(int argc, char * argv[]) \
{ \
	return submain(argc, argv); \
}
#else
#define INNOCENCE_MAIN \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) \
{ \
	System::Process::This thisProcess; \
	return submain(thisProcess.getArgCount(), thisProcess.getArgs()); \
}
#endif

#define INNOCENCE_DLL \
HANDLE threadHandle; \
extern "C" BOOL APIENTRY DllMain (HINSTANCE hModule, DWORD dwMsg, LPVOID lpReserved) \
{ \
	UNREFERENCED_PARAMETER( hModule ); \
	UNREFERENCED_PARAMETER( lpReserved ); \
	switch( dwMsg ) \
	{ \
		case DLL_PROCESS_ATTACH: \
			DisableThreadLibraryCalls( hModule ); \
			threadHandle = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)subdll, NULL, 0, 0 ); \
			return (!threadHandle) ? FALSE : TRUE; \
		case DLL_PROCESS_DETACH: \
			TerminateThread(threadHandle, EXIT_SUCCESS); \
			return TRUE; \
	} \
	return TRUE; \
}
