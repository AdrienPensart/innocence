#ifndef _PIPE_HPP_
#define _PIPE_HPP_

#include "Types.hpp"
#include "Stream.hpp"
#include "Exception.hpp"

#ifdef WIN32

#include <exception>
#include <string>
#include <windows.h>

namespace Network
{
    class PipeError : public Exception
    {
        public:

            PipeError(const std::string& argMsgError="");
            ~PipeError()throw();
            const char * what();

        private:

            std::string msgError;
    };

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

}

#endif // WIN32

#endif
