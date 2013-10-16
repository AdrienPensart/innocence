#pragma once

#include <common/Exception.hpp>

#include "Types.hpp"
#include "Stream.hpp"

#ifdef WIN32

#include <string>

namespace Network
{
    typedef Common::Exception PipeException;

    class Pipe : public Stream
    {
        public:

            Pipe();
            virtual ~Pipe();
            bool connect(const std::string& pipeName);
            bool listen(const std::string& pipeName);
            bool accept();
            bool disconnect();
            virtual int send(const char * object, int size);
            virtual int send(const char * object, int sizeOfObject, Timeout to);
            virtual int send(const std::string& object);
            virtual int send(const std::string& object, Timeout to);
            virtual int recv(char * object, int size);
            virtual int recv(char * object, int sizeOfObject, Timeout to);
            virtual int recv(std::string& object);
            virtual int recv(std::string& object, Timeout to);
			virtual int getBufferDataSize();

        protected:

            HANDLE hPipe;

    };

} // Network

#endif // WIN32
