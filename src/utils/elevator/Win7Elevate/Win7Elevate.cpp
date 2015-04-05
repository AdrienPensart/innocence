#include "Win7Elevate_Utils.h"
#include "Win7Elevate_Inject.h"

#include <string>
using namespace std;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	int argc = 0;
	wchar_t ** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
	/*
	TOKEN_ELEVATION_TYPE g_tet = TokenElevationTypeDefault;
	if (!W7EUtils::GetElevationType(&g_tet))
	{
		MessageBox(NULL, L"GetElevationType failed", L"Win7Elevate", MB_OK | MB_ICONERROR);
		return 0;
	}
	switch(g_tet)
	{
		case TokenElevationTypeDefault:
			MessageBox(NULL, L"UNKNOWN elevation level.", L"Win7Elevate", MB_OK | MB_ICONERROR);
			break;
		case TokenElevationTypeFull:
			MessageBox(NULL, L"RUNNING WITH elevation level.", L"Win7Elevate", MB_OK | MB_ICONERROR);
			break;
		case TokenElevationTypeLimited:
			MessageBox(NULL, L"RUNNING WITHOUT elevation level.", L"Win7Elevate", MB_OK | MB_ICONERROR);
			break;
		default:
			MessageBox(NULL, L"UNKNOW", L"Win7Elevate", MB_OK | MB_ICONERROR);
			break;
	}
	*/
	if(argc < 5) {
		//MessageBox(NULL, L"Nombre d'arguments invalide", L"Win7Elevate", MB_OK | MB_ICONERROR);
	} else {
		// 1er argument : nom du programme
		// 2eme argument : nom du processus a injecter
		wchar_t * szProc = argv[1];
		// 3eme argmument : pid du processus a injecter
		DWORD pid = _wtoi(argv[2]);
		// 4eme argument : dll d'injection privilege escalation
		wchar_t * szDll = argv[3];
		// 5eme argument : chemin et nom du programme a lancer en administrateur
		wchar_t * szCmd = argv[4];
		// on peut en deduire le repertoire dans lequel se trouve l'executable
		wstring szDir = argv[4];
		size_t last = szDir.find_last_of('\\');
		szDir = szDir.substr(0, last);
		// les arguments suivants sont les arguments du programme a lancer
		wstring szArgs;
		for(int index = 4; index < argc; index++) {
			szArgs += argv[index];
		}

		W7EInject::AttemptOperation(0, true, true, pid, szProc, szCmd, szArgs.c_str(), szDir.c_str(), szDll);
	}
	LocalFree(argv);

	return 0;
}
