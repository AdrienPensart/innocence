#include "Uac.hpp"
#include <common/Log.hpp>

#include <winnt.h>

namespace System
{
    bool GetElevationType(TOKEN_ELEVATION_TYPE * ptet)
	{
		bool bResult = false;
		HANDLE hToken = NULL;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		{
			DWORD dwReturnLength = 0;
			if (GetTokenInformation(hToken, TokenElevationType, ptet, sizeof(*ptet), &dwReturnLength ))
			{
				bResult = true;
			}
			else
			{
				LOG << "GetTokenInformation failed : " + toString(GetLastError());
			}
			CloseHandle(hToken);
		}
		else
		{
			LOG << "OpenProcessToken failed : " + toString(GetLastError());
		}
		return bResult;
	}

	bool isUacActivated()
	{
		TOKEN_ELEVATION_TYPE g_tet = TokenElevationTypeDefault;
		if (!GetElevationType(&g_tet))
        {
            LOG << "GetElevationType failed";
            return false;
        }
        return g_tet != TokenElevationTypeDefault;
	}

	bool isAdministrator()
	{
		TOKEN_ELEVATION_TYPE g_tet = TokenElevationTypeDefault;
		if (!GetElevationType(&g_tet))
        {
            LOG << "GetElevationType failed";
            return false;
        }
        return g_tet == TokenElevationTypeFull;
	}

    std::string GetElevationType()
	{
		TOKEN_ELEVATION_TYPE g_tet = TokenElevationTypeDefault;
		if (!GetElevationType(&g_tet))
        {
            return "GetElevationType failed";
        }
        switch(g_tet)
        {
			case TokenElevationTypeDefault:
				return "Default elevation level";
				break;
			case TokenElevationTypeFull:
				return "Elevated Level";
				break;
			case TokenElevationTypeLimited:
				return "Running without elevation";
				break;
        }
        return "Unknow elevation level";
	}

	bool RunAsAdministrator(const std::string& exe, const std::string& dir, bool show)
	{
		SHELLEXECUTEINFO TempInfo = {0};
		TempInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
		TempInfo.fMask = 0;
		TempInfo.hwnd = NULL;
		TempInfo.lpVerb = "runas";
		TempInfo.lpFile = exe.c_str();
		TempInfo.lpParameters = "runasadmin";
		TempInfo.lpDirectory = dir.c_str();
		TempInfo.nShow = show ? SW_NORMAL : SW_HIDE;
		BOOL bRet = ShellExecuteEx(&TempInfo);
		return bRet != 0;
	}
    
}/* System */
