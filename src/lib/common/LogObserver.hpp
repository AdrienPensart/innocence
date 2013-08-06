#pragma once

#include "Message.hpp"

#include <network/UdpSocket.hpp>
#include <fstream>

namespace Common
{
	class LogObserver
    {
        public:

            virtual void update(const Message& message)=0;
    };

    class LogToFile : public LogObserver
    {
        public:

            LogToFile(const std::string& filepath);
            virtual void update(const Message& message);

        private:

			const std::string filepath;
            std::ofstream file;
    };

    class LogToNetwork : public LogObserver
    {
        public:

            LogToNetwork(const Network::Host& debug_server, const Network::Port& debug_port);
            virtual void update(const Message& message);

        private:

            Network::UdpSocket socket_udp;
    };

	class LogToCollector : public LogToNetwork
	{
		public:
            LogToCollector();
	};

	class LogToConsole : public LogObserver
    {
        public:

            LogToConsole(const std::string& title="");
            virtual void update(const Message& message);

        private:

            const std::string title;
    };

} // Common
