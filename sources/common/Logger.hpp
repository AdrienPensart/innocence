#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#include <string>
#include <vector>
#include <fstream>
#include <network/UdpSocket.hpp>
#include "Singleton.hpp"
#include "Convert.hpp"
#include "Observable.hpp"

namespace Common
{
    class LoggingObserver
    {
        public:

            virtual void update(const std::string& object)=0;
    };

    // Les messages seront placés dans un fichier
    class LoggingFile : public LoggingObserver
    {
        public:

            LoggingFile(const std::string& filepath);
            virtual void update(const std::string& object);

        private:

			const std::string filepath;
            std::ofstream file;
    };

    // Les messages seront envoyés sur le réseau
    class LoggingNetwork : public LoggingObserver
    {
        public:

            LoggingNetwork(const Network::Host& debug_server, const Network::Port& debug_port);
            virtual void update(const std::string& object);

        private:

            Network::UdpSocket socket_udp;
    };

    // Les messages seront affichés directement à l'utilisateur
    class LoggingMessage : public LoggingObserver
    {
        public:

            LoggingMessage(const std::string& title="");
            virtual void update(const std::string& object);

        private:

            const std::string title;
    };

    class Logger : public Singleton<Logger>, public Observable<LoggingObserver, std::string>
    {
        public:
            
            void trace();
            void setHeader(const std::string& header);
            void enterFunction(const std::string& func);
            void leaveFunction();
            Logger& operator << (const std::string& object);
        
        protected:

            Logger();

        private:
            
            friend class Singleton<Logger>;
            bool tracing;
            std::string header;
            std::vector<std::string> functions;
    };
    
	#define LOG	Common::Logger::instance()
    struct ScopedLog
    {
        ScopedLog(const std::string& log);
        ~ScopedLog();
        const std::string& msg;
    };
    
    struct FunctionLog
    {
        FunctionLog(const std::string& functionName);
        ~FunctionLog();
    };

} /* Common */

void SHOW_BOX(const std::string& title, const std::string& msg);
void SHOW_FATAL_ERROR(const std::string& msg);
void FATAL_ERROR(const std::string& msg);
void SHOW_LAST_ERROR();
#define SCOPED_LOG(x) Common::ScopedLog scoped_log((x));
#define FUNC_LOG(x) Common::FunctionLog function_log((x));
#define LOG_THIS_FUNCTION Common::FunctionLog function_log((__FUNCTION__));

#endif // _LOGGER_HPP_
