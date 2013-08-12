#include "Pipe.hpp"

#include <common/Utility.hpp>

#ifdef WIN32

namespace Network
{
	Pipe::Pipe():hPipe(0)
    {
    }

    Pipe::~Pipe()
    {
        disconnect();
        if(hPipe)
		{
			CloseHandle(hPipe);
        }
	}

    bool Pipe::connect(const std::string& pipeName)
    {
		if(hPipe)
		{
			return false;
		}
        hPipe = CreateFile(pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
        if (hPipe == INVALID_HANDLE_VALUE)
        {
             throw PipeException("Pipe connection failed (CreateFile) : " + Common::toString(GetLastError()));
        }
        return true;
    }

    bool Pipe::listen(const std::string& pipeName)
    {
        hPipe = CreateNamedPipe(
			pipeName.c_str(),             // pipe name
            PIPE_ACCESS_DUPLEX,       // read/write access
            PIPE_TYPE_MESSAGE |       // message type pipe
            PIPE_READMODE_MESSAGE |   // message-read mode
            PIPE_WAIT,                // blocking mode
            PIPE_UNLIMITED_INSTANCES, // max. instances
            DEFAULT_MAX_CHAR,         // output buffer size
            DEFAULT_MAX_CHAR,         // input buffer size
            NMPWAIT_USE_DEFAULT_WAIT, // client time-out
            NULL);                    // default security attribute
		if (hPipe == INVALID_HANDLE_VALUE)
        {
            throw PipeException("Pipe listen failed (CreateNamedPipe) : " + Common::toString(GetLastError()));
        }
        return true;
    }

	bool Pipe::accept()
	{
		bool bClientConnected = ConnectNamedPipe(hPipe, 0) != 0;
        if (!bClientConnected)
        {
			throw PipeException("Pipe accept failed (ConnectNamedPipe) : " + Common::toString(GetLastError()));
        }
        return true;
    }

    bool Pipe::disconnect()
    {
		if(!hPipe)
		{
			return false;
		}
        DisconnectNamedPipe(hPipe);
        return true;
    }

    int Pipe::send(const char * object, int size)
    {
		int bytesRead = 0;
        bool bResult = WriteFile(hPipe,object,size,(DWORD *)&bytesRead,0) != 0; // évite le warning C4800
        if ( (!bResult) || (size) != bytesRead)
        {
			throw PipeException("Pipe send failed (WriteFile) : " + Common::toString(GetLastError()));
        }
        FlushFileBuffers(hPipe);
        return bytesRead;
    }

    int Pipe::send(const char * object, int sizeOfObject, Timeout)
	{
		return send(object, sizeOfObject);
	}
    
	int Pipe::send(const std::string& object)
	{
		return Stream::send(object);
	}
    
	int Pipe::send(const std::string& object, Timeout to)
	{
		return Stream::send(object, to);
	}

    int Pipe::recv(char * object, int size)
    {
		int bytesRead = 0;
        bool bResult = ReadFile(hPipe, object,size,(DWORD *)&bytesRead,0) != 0;// évite le warning C4800
        if ( (!bResult) || (bytesRead==0))
        {
	        throw PipeException("Pipe recv failed (ReadFile) : " + Common::toString(GetLastError()));
        }
	    return bytesRead;
    }

	int Pipe::recv(char * object, int sizeOfObject, Timeout)
	{
		return recv(object, sizeOfObject);
	}

    int Pipe::recv(std::string& object)
	{
		return Stream::recv(object);
	}

    int Pipe::recv(std::string& object, Timeout to)
	{
		return Stream::recv(object, to);
	}

    int Pipe::getBufferDataSize()
    {
		return DEFAULT_MAX_CHAR;
    }
}

#endif
