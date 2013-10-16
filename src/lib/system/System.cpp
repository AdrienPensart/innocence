#include "System.hpp"

#include <common/Utility.hpp>
#include <common/WindowsWrapper.hpp>
#include <string>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>

#pragma comment(lib, "User32.lib")
#define BUFSIZE 256

namespace System
{
	std::string getComputerName()
	{
		DWORD size = MAX_COMPUTERNAME_LENGTH;
		char computerName [MAX_COMPUTERNAME_LENGTH];
		if(GetComputerName(computerName, &size))
		{
			return std::string(computerName, size);
		}
		return "";
	}

	std::string getCompleteComputerName()
	{
		DWORD size = MAX_COMPUTERNAME_LENGTH;
		char computerName [MAX_COMPUTERNAME_LENGTH];
		if(GetComputerNameEx(ComputerNameDnsFullyQualified, computerName, &size))
		{
			return std::string(computerName, size);
		}
		return "";
	}

	std::string getWindowsPath()
	{
		char winpath [MAX_PATH];
        unsigned int size_returned = 0;        
        if(size_returned = GetWindowsDirectory(winpath, MAX_PATH))
		{
			return std::string(winpath, size_returned);
		}
		return "";
	}

    void Token()
    {
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;
        OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
    }

    SystemVersion getSystemVersion()
    {
        OSVERSIONINFOEX osvi;
        BOOL bOsVersionInfoEx;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        if(!(bOsVersionInfoEx=GetVersionEx((OSVERSIONINFO *)&osvi)))
        {
            osvi.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
            if (!GetVersionEx((OSVERSIONINFO *)&osvi) )
                return OS_ERROR;
        }
        
        if(osvi.dwMajorVersion==3 && osvi.dwMinorVersion==51)
            return OS_WIN32_WINDOWS_NT_3_51;
        if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
            return OS_WIN32_WINDOWS_NT_4_0;
        if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
            return OS_WIN32_WINDOWS_95;
        if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==10 && osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
            return OS_WIN32_WINDOWS_98;
        if(osvi.dwMajorVersion==4 && osvi.dwMinorVersion==90 && osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
            return OS_WIN32_WINDOWS_Me;
        if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
            return OS_WIN32_WINDOWS_2000;
        if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==1 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
            return OS_WIN32_WINDOWS_XP;
        if(osvi.dwMajorVersion==5 && osvi.dwMinorVersion==2 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
            return OS_WIN32_WINDOWS_SERVER_2003_FAMILY;
        if(osvi.dwMajorVersion==6 && osvi.dwMinorVersion==0 && osvi.dwPlatformId==VER_PLATFORM_WIN32_NT)
            return OS_WIN32_WINDOWS_VISTA;
		if(VER_PLATFORM_WIN32_NT == osvi.dwPlatformId && osvi.dwMinorVersion == 1 && osvi.wProductType == VER_NT_WORKSTATION)
			return OS_WIN32_WINDOWS_SEVEN;
        return OS_UNKNOW;
    }

    std::string getSystemVersionString()
    {
        switch(getSystemVersion())
        {
            case OS_ERROR:
                return "Error SystemVersion";
            case OS_WIN32_WINDOWS_NT_3_51:
                return "Windows NT 3.51";
            case OS_WIN32_WINDOWS_NT_4_0:
                return "Windows NT 3.51";
            case OS_WIN32_WINDOWS_95:
                return "Windows 95";
            case OS_WIN32_WINDOWS_98:
                return "Windows 98";
            case OS_WIN32_WINDOWS_Me:
                return "Windows Me";
            case OS_WIN32_WINDOWS_2000:
                return "Windows 2000";
            case OS_WIN32_WINDOWS_XP:
                return "Windows Xp";
            case OS_WIN32_WINDOWS_SERVER_2003_FAMILY:
                return "Windows Server 2003 Family";
            case OS_WIN32_WINDOWS_VISTA:
                return "Windows Vista All Versions";
			case OS_WIN32_WINDOWS_SEVEN:
				return "Windows Seven All Versions";
            case OS_UNKNOW:
                return "Unknow system";
            default:
                break;
        }
        return "Unable to determine SystemVersion";
    }

	bool is64BitWindows()
	{
		#if defined(_WIN64)
			return TRUE;  // 64-bit programs run only on Win64
		#elif defined(_WIN32)
			// 32-bit programs run on both 32-bit and 64-bit Windows
			// so must sniff
			BOOL f64 = FALSE;
			return IsWow64Process(GetCurrentProcess(), &f64) && f64;
		#else
			return FALSE; // Win64 does not support Win16
		#endif
	}

    bool shutdown()
    {
        Token();
        return ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0) != 0; 
    }

    bool reboot()
    {
        Token();
        return ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0) != 0;
    }

    bool logout()
    {
        Token();
        return ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0) != 0; 
    }

    bool hibernate()
    {
        Token();
        return SetSystemPowerState(TRUE, TRUE) != 0;
    }

	typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
	typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);
	/*
	std::string getSystemVersionString()
	{
		std::string systemString = "Unknow System!";
		OSVERSIONINFOEX osvi;
		SYSTEM_INFO si;
		PGNSI pGNSI;
		PGPI pGPI;
		BOOL bOsVersionInfoEx;
		DWORD dwType;

		ZeroMemory(&si, sizeof(SYSTEM_INFO));
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
		{
			return systemString;
		}
		pGNSI = (PGNSI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"GetNativeSystemInfo");
		if(NULL != pGNSI)
		{
			pGNSI(&si);
		}
		else
		{
			GetSystemInfo(&si);
		}
		if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && osvi.dwMajorVersion > 4 )
		{
			systemString = "Microsoft ";
			if ( osvi.dwMajorVersion == 6 )
			{
				if( osvi.dwMinorVersion == 0 )
				{
					if( osvi.wProductType == VER_NT_WORKSTATION )
					{
						systemString += "Windows Vista ";
					}
					else
					{
						systemString += "Windows Server 2008 ";
					} 
				}

				if ( osvi.dwMinorVersion == 1 )
				{
					if( osvi.wProductType == VER_NT_WORKSTATION )
					{
						systemString += "Windows 7 ";
					}
					else
					{
						systemString += "Windows Server 2008 R2 ";
					}
				}
	         
				pGPI = (PGPI) GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")),"GetProductInfo");
				pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);
				switch( dwType )
				{
					case PRODUCT_ULTIMATE:
						systemString += "Ultimate Edition";
						break;
					case PRODUCT_HOME_PREMIUM:
						systemString += "Home Premium Edition";
						break;
					case PRODUCT_HOME_BASIC:
						systemString += "Home Basic Edition";
						break;
					case PRODUCT_ENTERPRISE:
						systemString += "Enterprise Edition";
						break;
					case PRODUCT_BUSINESS:
						systemString += "Business Edition";
						break;
					case PRODUCT_STARTER:
						systemString += "Starter Edition";
						break;
					case PRODUCT_CLUSTER_SERVER:
						systemString += "Cluster Server Edition";
						break;
					case PRODUCT_DATACENTER_SERVER:
						systemString += "Datacenter Edition";
						break;
					case PRODUCT_DATACENTER_SERVER_CORE:
						systemString += "Datacenter Edition (core installation)";
						break;
					case PRODUCT_ENTERPRISE_SERVER:
						systemString += "Enterprise Edition";
						break;
					case PRODUCT_ENTERPRISE_SERVER_CORE:
						systemString += "Enterprise Edition (core installation)";
						break;
					case PRODUCT_ENTERPRISE_SERVER_IA64:
						systemString += "Enterprise Edition for Itanium-based Systems";
						break;
					case PRODUCT_SMALLBUSINESS_SERVER:
						systemString += "Small Business Server";
						break;
					case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
						systemString += "Small Business Server Premium Edition";
						break;
					case PRODUCT_STANDARD_SERVER:
						systemString += "Standard Edition";
						break;
					case PRODUCT_STANDARD_SERVER_CORE:
						systemString += "Standard Edition (core installation)";
						break;
					case PRODUCT_WEB_SERVER:
						systemString += "Web Server Edition";
						break;
				}
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
			{
				if( GetSystemMetrics(SM_SERVERR2) )
				{
					systemString += "Windows Server 2003 R2, ";
				}
				else if ( osvi.wSuiteMask==VER_SUITE_STORAGE_SERVER )
				{
					systemString += "Windows Storage Server 2003";
				}
				else if( osvi.wProductType == VER_NT_WORKSTATION && si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
				{
					systemString += "Windows XP Professional x64 Edition";
				}
				else
				{
					systemString += "Windows Server 2003, ";
				}

				if(osvi.wProductType != VER_NT_WORKSTATION )
				{
					if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						{
							systemString += "Datacenter Edition for Itanium-based Systems";
						}
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						{
							systemString += "Enterprise Edition for Itanium-based Systems";
						}
					}
					else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
					{
						if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						{
							systemString += "Datacenter x64 Edition";
						}
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						{
							systemString += "Enterprise x64 Edition";
						}
						else
						{
							systemString += "Standard x64 Edition";
						}
					}
					else
					{
						if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						{
							systemString += "Compute Cluster Edition";
						}
						else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						{
							systemString += "Datacenter Edition";
						}
						else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						{
							systemString += "Enterprise Edition";
						}
						else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						{
							systemString += "Web Edition";
						}
						else
						{
							systemString += "Standard Edition";
						}
					}
				}
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
			{
				systemString += "Windows XP ";
				if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				{
					systemString += "Home Edition";
				}
				else
				{
					systemString += "Professional";
				}
			}

			if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
			{
				systemString += "Windows 2000 ";

				if ( osvi.wProductType == VER_NT_WORKSTATION )
				{
					systemString += "Professional";
				}
				else 
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					{
						systemString += "Datacenter Server";
					}
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					{
						systemString += "Advanced Server";
					}
					else
					{
						systemString += "Server";
					}
				}
			}

			if( _tcslen(osvi.szCSDVersion) > 0 )
			{
				systemString += " ";
				systemString += osvi.szCSDVersion;
			}

			systemString += " (build ";
			systemString += Common::toString(osvi.dwBuildNumber);
			systemString += ")";
			if ( osvi.dwMajorVersion >= 6 )
			{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					systemString += ", 64-bit";
				}
				else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				{
					systemString += ", 32-bit";
				}
			}
		}
		return systemString;
	}
	*/	
} /* System */
