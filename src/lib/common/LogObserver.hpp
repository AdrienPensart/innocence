#ifndef _LOG_OBSERVER_
#define _LOG_OBSERVER_

#include <network/UdpSocket.hpp>
#include <fstream>

namespace Common
{
	class LogObserver
    {
        public:

            virtual void update(const std::string& object)=0;
    };

    class LogToFile : public LogObserver
    {
        public:

            LogToFile(const std::string& filepath);
            virtual void update(const std::string& object);

        private:

			const std::string filepath;
            std::ofstream file;
    };

    class LogToNetwork : public LogObserver
    {
        public:

            LogToNetwork(const Network::Host& debug_server, const Network::Port& debug_port);
            virtual void update(const std::string& object);

        private:

            Network::UdpSocket socket_udp;
    };

	class LogToConsole : public LogObserver
    {
        public:

            LogToConsole(const std::string& title="");
            virtual void update(const std::string& object);

        private:

            const std::string title;
    };
} // Common

#endif // _LOG_OBSERVER_
