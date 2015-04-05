#include <windows.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     ) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		int argc = 0;
		wchar_t **argv = CommandLineToArgvW(GetCommandLine(), &argc);

		if (argc != 4) {
			ExitProcess(-69);
		} else {
			wchar_t *szCmd  = argv[1];
			wchar_t *szDir  = argv[2];
			wchar_t *szArgs = argv[3];
			STARTUPINFO startupInfo = {0};
			startupInfo.cb = sizeof(startupInfo);
			PROCESS_INFORMATION processInfo = {0};
			if (CreateProcess(szCmd, szArgs, NULL, NULL, FALSE, 0, NULL, szDir, &startupInfo, &processInfo)) {
				CloseHandle(processInfo.hProcess);
				CloseHandle(processInfo.hThread);
			}
			ExitProcess(-69);
		}
	}
	break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
