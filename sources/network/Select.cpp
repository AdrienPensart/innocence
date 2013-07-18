#include "Select.hpp"

namespace Network
{
    bool SelectSocket::SelectOnRead(Socket& socket, Timeout& to)
    {
        return LowLevelSelect(socket,READFS,to);
    }
    
    bool SelectSocket::SelectOnWrite(Socket& socket, Timeout& to)
    {
        return LowLevelSelect(socket,WRITEFS,to);
    }
	
    bool SelectSocket::SelectOnExcept(Socket& socket, Timeout& to)
    {
        return LowLevelSelect(socket,EXCEPTFS,to);
    }

    bool SelectSocket::LowLevelSelect(Socket& socket,SelectType type, Timeout& to)
    {	        
        fd_set fs;
        FD_ZERO(&fs);
        FD_SET(socket.getDescriptor(), &fs);
        int retval = 0;
			
        switch(type)
        {
            case READFS:
                retval = select(socket.getDescriptor()+1, &fs, 0, 0, &to.c_struct());
                break;
            case WRITEFS:
                retval = select(socket.getDescriptor()+1, 0, &fs, 0, &to.c_struct());
                break;
            case EXCEPTFS:
                retval = select(socket.getDescriptor()+1, 0, 0, &fs, &to.c_struct());
                break;
        }
        // erreur
        if (retval == SOCKET_ERROR)
        {
			throw SocketException(socket.getDescriptor(), "Select:SOCKET_ERROR");
        }
        else if (retval)
        {
            return true;
        }
        // sinon il n'y a pas de données.
        return false;
    }
}

