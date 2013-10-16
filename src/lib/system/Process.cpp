#include "Process.hpp"
#include "File.hpp"
#include "Uac.hpp"

#include <tchar.h>
#include <tlhelp32.h>
#include <shellapi.h>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <list>
#include <log/Log.hpp>
#include <common/Exception.hpp>
using namespace std;

namespace System
{
	namespace Process
	{
		DWORD This::getPid()
		{
			return GetCurrentProcessId();
		}

		DWORD This::getParentPid()
		{
			return GetParentPid(getPid());
		}

		ProcessEntry GetParentProcessEntry(DWORD pid)
		{
			ProcessEntry pe;
			HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe32 = { 0 };
			pe32.dwSize = sizeof(PROCESSENTRY32);

			if( Process32First(h, &pe32))
			{
    			do
				{
    				if (pe32.th32ProcessID == pid)
					{
						pe.first = pe32.th32ParentProcessID;
						pe.second = pe32.szExeFile;
    				}
    			} while( Process32Next(h, &pe32));
			}
			CloseHandle(h);
			return pe;
		}

		DWORD GetParentPid(DWORD pid)
		{
			DWORD ppid = -1;
			HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pe = { 0 };
			pe.dwSize = sizeof(PROCESSENTRY32);

			if( Process32First(h, &pe)) {
    			do {
    				if (pe.th32ProcessID == pid) {
    					ppid = pe.th32ParentProcessID;
    				}
    			} while( Process32Next(h, &pe));
			}
			CloseHandle(h);
			return ppid;
		}

		std::string This::getPath()
		{
			HMODULE hModule = GetModuleHandle(NULL);
			CHAR path[MAX_PATH];
			GetModuleFileName(hModule, path, MAX_PATH);
			return path;
		}

		This::This()
			:argc(0), argvw(0), argv(0)
		{
			// récuperation des arguments au programme
			argvw = CommandLineToArgvW(GetCommandLineW(), &argc);
			if(argvw == NULL)
			{
				throw Common::Exception("CommandLineToArgvW failed : " + GetLastError());
			}

			// Conversion en chaine multi-octets
			argv = new char * [argc];
			for(int index = 0; index < argc; index++)
			{
				size_t stringSize = wcslen(argvw[index]);
				argv[index] = new char [stringSize+1];
				wcstombs(argv[index], argvw[index], stringSize+1);
			}

			programPath = getPath();
			programName = programPath;
			programDir = programPath;
			programName = GetFileName(programName);
			programDir = GetFileDir(programDir);
		}

		This::~This()
		{
			LocalFree(argvw);
			for(int index = 0; index < argc; index++)
			{
				delete argv[index];
			}
			delete argv;
		}

		void This::killHierarchy()
		{
			LOG << "Killing current process hierarchy";
			ProcessEntry pe;
			DWORD nextPid = getPid();
			std::string programName = getProgramName();
			std::transform(programName.begin(), programName.end(), programName.begin(), ::tolower);

			std::list<DWORD> processChain;

			// aggregate process tree
			while(true)
			{
				processChain.push_back(nextPid);
				pe = GetParentProcessEntry(nextPid);
				std::transform(pe.second.begin(), pe.second.end(), pe.second.begin(), ::tolower);
				//LOG << "Comparing " + pe.second + "(" + Common::toString(pe.first) + ") and " + programName + "(" + Common::toString(nextPid) + ")";
				if(pe.second != programName)
				{
					break;
				}
				nextPid = pe.first;
			}

			// killing all processes in reverse order
			for(std::list<DWORD>::reverse_iterator i = processChain.rbegin(); i != processChain.rend(); i++)
			{
				LOG << "Killing process with pid " + Common::toString(*i);
				System::Process::KillProcess(*i);
			}
		}

		bool This::runAsAdmin()
		{
			if(!isAdministrator())
			{
				LOG << "Try to run as administrator " + programPath + " as admin";
				RunAsAdministrator(programName, programDir, true);
				return false;
			}
			return true;
		}

		const std::string& This::getProgramPath()
		{
			return programPath;
		}

		const std::string& This::getProgramName()
		{
			return programName;
		}

		const std::string& This::getProgramDir()
		{
			return programDir;
		}

		int& This::getArgCount()
		{
			return argc;
		}

		const std::string This::getArg(unsigned int index)
		{
			return argv[index];
		}

		char ** This::getArgs()
		{
			return argv;
		}

		Launcher::Launcher(const std::string& executable, const std::string& args, bool show)
		{
			PROCESS_INFORMATION pInfo;
			STARTUPINFOA sInfo;
			sInfo.cb = sizeof(STARTUPINFOA);
			sInfo.lpReserved = NULL;
			sInfo.lpReserved2 = NULL;
			sInfo.cbReserved2 = 0;
			sInfo.lpDesktop = NULL;
			sInfo.lpTitle = NULL;
			sInfo.dwFlags = 0;
			sInfo.dwX = 0;
			sInfo.dwY = 0;
			sInfo.dwFillAttribute = 0;
			sInfo.wShowWindow = SW_HIDE;
 
			memset (&sInfo, 0, sizeof(sInfo));
			sInfo.cb = sizeof(sInfo);

			string cmdLine = executable + " " + args;
			BOOL createProcessResult = CreateProcess(NULL, LPSTR(cmdLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
			/*
			LOG << "Creating process " + executable + " " + args;
			memset(&ExecuteInfo, 0, sizeof(ExecuteInfo));
			ExecuteInfo.cbSize       = sizeof(ExecuteInfo);
			ExecuteInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
			ExecuteInfo.hwnd         = 0;
			ExecuteInfo.lpVerb       = 0;
			ExecuteInfo.lpFile       = executable.c_str();
			ExecuteInfo.lpParameters = args.c_str();
			ExecuteInfo.lpDirectory  = 0;
			ExecuteInfo.nShow        = show ? SW_SHOW : SW_HIDE;
			ExecuteInfo.hInstApp     = 0;
			*/
			//CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
			/*
			if(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE) != S_OK)
			{
				throw Common::Exception("CoInitializeEx failed");
			}
			createProcessResult = ShellExecuteEx(&ExecuteInfo);
			*/

			if(!createProcessResult)
			{
				running = false;
				throw Common::Exception("Creating the process " + executable + " failed : " + Common::toString(GetLastError()));
			}
			else
			{
				running = true;
				pid = pInfo.dwProcessId;
				process = pInfo.hProcess;
			}
		}

		DWORD Launcher::getPid()
		{
			//return GetProcessId(ExecuteInfo.hProcess);
			return pid;
		}

		DWORD Launcher::wait()
		{
			DWORD exitCode = EXIT_FAILURE;
			if(isRunning())
			{
				WaitForSingleObject(process,INFINITE);
				GetExitCodeProcess(process, &exitCode);
			}
			return exitCode;
		}

		void Launcher::kill()
		{
			KillProcess(getPid());
		}

		bool Launcher::isRunning()
		{
			return running;
		}

		bool Launcher::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
		{
			TOKEN_PRIVILEGES tp;
			LUID luid;

			if ( !LookupPrivilegeValue(NULL, lpszPrivilege, &luid) )
			{
				LOG << "LookupPrivilegeValue failed : " + Common::toString(GetLastError());
				return false;
			}

			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			if (bEnablePrivilege)
			{
				tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			}
			else
			{
				tp.Privileges[0].Attributes = 0;
			}

			if ( !AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(TOKEN_PRIVILEGES),(PTOKEN_PRIVILEGES) NULL,(PDWORD) NULL) )
			{
				LOG << "AdjustTokenPrivileges failed : " + Common::toString(GetLastError());
				return false;
			}
			if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
			{
				LOG << "The token does not have the specified privilege";
				return false;
			}
			return true;
		}

		bool GetProcessList(Process::Map& mapProcs)
		{
			bool bResult = false;
			mapProcs.clear();

			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hSnapshot == INVALID_HANDLE_VALUE)
			{
				LOG << "CreateToolhelp32Snapshot failed : " + Common::toString(GetLastError());
			}
			else
			{
				bool bFirst = true;
				PROCESSENTRY32 pe;
				while(true)
				{
					ZeroMemory(&pe, sizeof(pe));
					pe.dwSize = sizeof(pe);

					BOOL bPR = FALSE;
					if (bFirst)
					{
						bFirst = false;
						bPR = Process32First(hSnapshot, &pe);
					}
					else
					{
						bPR = Process32Next(hSnapshot, &pe);
					}

					if (!bPR)
					{
						DWORD dwErr = GetLastError();

						if (ERROR_NO_MORE_FILES != dwErr)
						{
							LOG << "Process32Next/First failed";
						}
						else if (mapProcs.empty())
						{
							LOG << "Process32Next/First returned nothing";
						}
						else
						{
							bResult = true;
						}
						break;
					}

					// on ajoute à la liste uniquement les processus dont on peut obtenir les
					// droits dessus
					HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
					if (hProc != 0)
					{
						CloseHandle(hProc);
						mapProcs.insert( std::make_pair( pe.th32ProcessID, pe.szExeFile ) );
					}
				}
	
				CloseHandle(hSnapshot);
			}
			return bResult;
		}

		typedef BOOL (CALLBACK * PFNENUMPROC)(DWORD dwProcessId, LPCTSTR pszProcessName, PDWORD pdwPID, LPCTSTR pszName);
		BOOL _EnumProcesses(PFNENUMPROC pfnEnumProc, LPCTSTR pszName, PDWORD pdwPID);
		BOOL CALLBACK GetPIDFromNameCallback(DWORD dwProcessId, LPCTSTR pszProcessName, PDWORD pdwPID, LPCTSTR pszNameToFind);
		BOOL CALLBACK EnumProcessCallback(DWORD dwProcessId, LPCTSTR pszProcessName, PDWORD pdwPID, LPCTSTR pszName);
		BOOL SetDebugPrivileges(VOID);

		DWORD GetPidFromName(const std::string& name)
		{
			DWORD dwPID = 0;
			_EnumProcesses(GetPIDFromNameCallback, name.c_str(), &dwPID);
			return dwPID;
		}

		std::string processes;
		std::string GetAllRunningProcess()
		{
			_EnumProcesses(EnumProcessCallback, 0, 0);
			std::string current_proc_list = processes;
			processes.erase(processes.begin(), processes.end());
			return current_proc_list;
		}

		bool KillProcess(const std::string& name)
		{
			return KillProcess(GetPidFromName(name));
		}

		bool KillProcess(DWORD pid)
		{
			HANDLE hProcess;
			if ((hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid)) != 0)
			{
				if (TerminateProcess(hProcess, 0) != 0)
				{
					CloseHandle(hProcess);
					return true;
				}
				else
				{
					LOG << "KillProcess (TerminateProcess) failed : " + Common::toString(GetLastError());
				}
			}
			else
			{
				LOG << "KillProcess (OpenProcess) failed : " + Common::toString(GetLastError());
			}
			return false;
		}

		#ifndef offsetof
		#   define offsetof(a, b)   ((size_t)&(((a *)0)->b))
		#endif

		BOOL WINAPI EnumProcessesToolHelp(PFNENUMPROC pfnEnumProc, LPCTSTR pszName, PDWORD pdwPID)
		{
			typedef HANDLE (WINAPI *CREATESNAPSHOT) (DWORD, DWORD);
			typedef BOOL   (WINAPI *PROCESSWALK)    (HANDLE, LPPROCESSENTRY32);

			HINSTANCE       hKernel;
			CREATESNAPSHOT  CreateToolhelp32Snapshot;
			PROCESSWALK     Process32First;
			PROCESSWALK     Process32Next;
			HANDLE          hSnapshot;
			PROCESSENTRY32  pe32;
			DWORD           dwPID;
			LPTSTR          pszProcessName;

			// Callback function must be defined
			if (!pfnEnumProc)
				return FALSE;

			// Initialize PID to -1
			if (pdwPID)
				*pdwPID = 0;

			// Get kernel module handle
			if (!(hKernel = GetModuleHandle(TEXT("Kernel32.dll"))))
				return FALSE;

			// We must link to these functions explicitly.
			// Otherwise it will fail on some versions of Windows NT
			// which doesn't have Toolhelp functions defined in Kernel32.
			CreateToolhelp32Snapshot = (CREATESNAPSHOT) GetProcAddress(hKernel, "CreateToolhelp32Snapshot");
		#ifdef UNICODE
			Process32First = (PROCESSWALK) GetProcAddress(hKernel, "Process32FirstW");
			Process32Next = (PROCESSWALK) GetProcAddress(hKernel, "Process32NextW");
		#else
			Process32First = (PROCESSWALK) GetProcAddress(hKernel, "Process32First");
			Process32Next = (PROCESSWALK) GetProcAddress(hKernel, "Process32Next");
		#endif

			// Error
			if (!CreateToolhelp32Snapshot || !Process32First || !Process32Next)
			{
				SetLastError(ERROR_PROC_NOT_FOUND);
				return FALSE;
			}

			// Create a snapshot
			hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (hSnapshot == INVALID_HANDLE_VALUE)
				return FALSE;

			// First process
			pe32.dwSize = sizeof(pe32);
			if (!Process32First(hSnapshot, &pe32))
			{
				CloseHandle(hSnapshot);
				return FALSE;
			}

			// Walk through all processes
			do
			{
				if (pe32.dwSize > offsetof(PROCESSENTRY32, szExeFile))
				{
					// Get process name (without path)
					pszProcessName = _tcsrchr(pe32.szExeFile, TEXT('\\'));
					if (pszProcessName)
						pszProcessName++;
					else
						pszProcessName = pe32.szExeFile;
					dwPID = pe32.th32ProcessID;
				}
				else
				{
					pszProcessName = NULL;
					dwPID = 0;
				}

				// Call callback function
				// If returned FALSE stop enumeration
				if (!pfnEnumProc(dwPID, pszProcessName, pdwPID, pszName))
					break;

				pe32.dwSize = sizeof(pe32);
			} while (Process32Next(hSnapshot, &pe32));

			CloseHandle(hSnapshot);

			return TRUE;
		}

		BOOL WINAPI EnumProcessesNtQuerySystemInformation(PFNENUMPROC pfnEnumProc, LPCTSTR pszName, PDWORD pdwPID)
		{
			typedef LONG    NTSTATUS;
			typedef LONG    KPRIORITY;

			#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
			#define STATUS_INFO_LENGTH_MISMATCH    ((NTSTATUS)0xC0000004L)
			#define SystemProcessesAndThreadsInformation    5

			typedef struct _CLIENT_ID
			{
				DWORD       UniqueProcess;
				DWORD       UniqueThread;
			} CLIENT_ID;

			typedef struct UNICODE_STRING
			{
				USHORT      Length;
				USHORT      MaximumLength;
				PWSTR       Buffer;
			} UNICODE_STRING;

			typedef struct _VM_COUNTERS 
			{
				SIZE_T      PeakVirtualSize;
				SIZE_T      VirtualSize;
				ULONG       PageFaultCount;
				SIZE_T      PeakWorkingSetSize;
				SIZE_T      WorkingSetSize;
				SIZE_T      QuotaPeakPagedPoolUsage;
				SIZE_T      QuotaPagedPoolUsage;
				SIZE_T      QuotaPeakNonPagedPoolUsage;
				SIZE_T      QuotaNonPagedPoolUsage;
				SIZE_T      PagefileUsage;
				SIZE_T      PeakPagefileUsage;
			} VM_COUNTERS;

			typedef struct _SYSTEM_THREADS 
			{
				LARGE_INTEGER   KernelTime;
				LARGE_INTEGER   UserTime;
				LARGE_INTEGER   CreateTime;
				ULONG           WaitTime;
				PVOID           StartAddress;
				CLIENT_ID       ClientId;
				KPRIORITY       Priority;
				KPRIORITY       BasePriority;
				ULONG           ConTEXTSwitchCount;
				LONG            State;
				LONG            WaitReason;
			} SYSTEM_THREADS, * PSYSTEM_THREADS;

			// NOTE: SYSTEM_PROCESSES structure is different on NT 4 and Win2K
			typedef struct _SYSTEM_PROCESSES 
			{
				ULONG           NextEntryDelta;
				ULONG           ThreadCount;
				ULONG           Reserved1[6];
				LARGE_INTEGER   CreateTime;
				LARGE_INTEGER   UserTime;
				LARGE_INTEGER   KernelTime;
				UNICODE_STRING  ProcessName;
				KPRIORITY       BasePriority;
				ULONG           ProcessId;
				ULONG           InheritedFromProcessId;
				ULONG           HandleCount;
				ULONG           Reserved2[2];
				VM_COUNTERS     VmCounters;
			#if _WIN32_WINNT >= 0x500
				IO_COUNTERS     IoCounters;
			#endif
				SYSTEM_THREADS  Threads[1];
			} SYSTEM_PROCESSES, * PSYSTEM_PROCESSES;

			typedef LONG (WINAPI *NTQUERYSYSTEMINFORMATION)(UINT SystemInformationClass, PVOID SystemInformation, ULONG SystemInformationLength, PULONG ReturnLength);

			NTQUERYSYSTEMINFORMATION    NtQuerySystemInformation;
			PSYSTEM_PROCESSES           pInfo;
			HINSTANCE                   hNTDll;
			PCWSTR                      pszProcessName;
			DWORD                       dwPID;
			ULONG                       BufferLen = 0x8000;
			LPVOID                      pBuffer = NULL;
			LONG                        Status;

		#ifndef UNICODE
			CHAR                        szProcessName[MAX_PATH];
		#endif

			if (!pfnEnumProc)
			{
				return FALSE;
			}

			if (pdwPID)
			{
				*pdwPID = 0;
			}

			if (!(hNTDll = LoadLibrary(TEXT("NTDLL.DLL"))))
			{
				return FALSE;
			}

			if (!(NtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION)GetProcAddress(hNTDll, "NtQuerySystemInformation")))
			{
				FreeLibrary(hNTDll);
				SetLastError(ERROR_PROC_NOT_FOUND);
				return FALSE;
			}
			do
			{
				if (!(pBuffer = malloc(BufferLen)))
				{
					FreeLibrary(hNTDll);
					SetLastError(ERROR_NOT_ENOUGH_MEMORY);
					return FALSE;
				}

				Status = NtQuerySystemInformation(SystemProcessesAndThreadsInformation,
												  pBuffer, BufferLen, NULL);

				if (Status == STATUS_INFO_LENGTH_MISMATCH)
				{
					free(pBuffer);
					BufferLen *= 2;
				}
				else if (!NT_SUCCESS(Status))
				{
					free(pBuffer);
					FreeLibrary(hNTDll);
					return FALSE;
				}
			}
			while (Status == STATUS_INFO_LENGTH_MISMATCH);

			pInfo = (PSYSTEM_PROCESSES)pBuffer;
			for (;;)
			{
				pszProcessName = pInfo->ProcessName.Buffer;
				if (pszProcessName == NULL)
					pszProcessName = L"Idle";
				dwPID = pInfo->ProcessId;

				// Appel de la fonction de callback

		#ifdef UNICODE
				if (!pfnEnumProc(dwPID, (LPCTSTR)pszProcessName, pdwPID, pszName))
				{
					break;
				}
		#else
				WideCharToMultiByte(CP_ACP, 0, pszProcessName, -1, szProcessName, MAX_PATH, NULL, NULL);
				if (!pfnEnumProc(dwPID, (LPCTSTR)szProcessName, pdwPID, pszName))
				{
					break;
				}
		#endif

				if (pInfo->NextEntryDelta == 0)
				{
					break;
				}
				pInfo = (PSYSTEM_PROCESSES)(((PUCHAR)pInfo) + pInfo->NextEntryDelta);
			}

			free(pBuffer);
			FreeLibrary(hNTDll);
			return TRUE;
		}

		BOOL _EnumProcesses(PFNENUMPROC pfnEnumProc, LPCTSTR pszName, PDWORD pdwPID)
		{
			OSVERSIONINFO   info;
			info.dwOSVersionInfoSize = sizeof(info);
			GetVersionEx(&info);

			// selon la version de windows, on utilise Toolhelp ou NtQuerySystemInformation
			if (info.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
				return EnumProcessesToolHelp(pfnEnumProc, pszName, pdwPID);
			else if (info.dwPlatformId == VER_PLATFORM_WIN32_NT)
				return EnumProcessesNtQuerySystemInformation(pfnEnumProc, pszName, pdwPID);
			else
				return FALSE;
		}

		BOOL SetDebugPrivileges(VOID)
		{
			DWORD dwPID;
			HANDLE hProcess;
			HANDLE hToken;
			LUID Luid;
			TOKEN_PRIVILEGES tpDebug;
			dwPID = GetCurrentProcessId();
			if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) == NULL)
				return FALSE;
			if (OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken) == 0)
				return FALSE;
			if ((LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid)) == 0)
				return FALSE;
			tpDebug.PrivilegeCount = 1;
			tpDebug.Privileges[0].Luid = Luid;
			tpDebug.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if ((AdjustTokenPrivileges(hToken, FALSE, &tpDebug, sizeof(tpDebug), NULL, NULL)) == 0)
				return FALSE;
			if (GetLastError() != ERROR_SUCCESS)
				return FALSE;
			CloseHandle(hToken);
			CloseHandle(hProcess);
			return TRUE;
		}

		BOOL CALLBACK EnumProcessCallback(DWORD dwProcessId, LPCTSTR pszProcessName, PDWORD pdwPID, LPCTSTR pszName)
		{
			processes += (char *)pszProcessName;
			processes += '\n';
			return true;
		}

		BOOL CALLBACK GetPIDFromNameCallback(DWORD dwProcessId, LPCTSTR pszProcessName, PDWORD pdwPID, LPCTSTR pszNameToFind)
		{
			if (_tcsicmp(pszProcessName, pszNameToFind) == 0)
			{
				// on a trouvé le processus
				*pdwPID = dwProcessId;
				return FALSE;
			}
			else
			// on continue l'énumération
			return TRUE;
		}
	} // Process
} // System

