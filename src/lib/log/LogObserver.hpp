#pragma once

#include <network/TcpClient.hpp>
#include <fstream>

namespace Log
{
	class Message;

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

            LogToNetwork(const Network::Host& loggerIp, const Network::Port& loggerPort);
            virtual void update(const Message& message);
			Network::Stream& getStream();

        private:

			bool connected;
            Network::TcpClient socket;
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

} // Log
