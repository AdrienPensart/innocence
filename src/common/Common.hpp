#pragma once

#include <system/Process.hpp>

// hack to quote string at compile time
#define Q(x) #x
#define QUOTE(x) Q(x)

// hack to comment log lines when in release mode
#define COMMENT SLASH(/)
#define SLASH(s) /##s

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
