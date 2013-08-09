#include "SelectSet.hpp"

namespace Network
{
    bool SelectSet::SelectOnRead(Socket& socket, Timeout& to)
    {
        return LowLevelSelect(socket, READFS, to);
    }
    
	SocketSet SelectSet::SelectOnRead(SocketSet& socketSet, Timeout& to)
	{
		return LowLevelSelect(socketSet, READFS, to);
	}

    bool SelectSet::SelectOnWrite(Socket& socket, Timeout& to)
    {
        return LowLevelSelect(socket, WRITEFS, to);
    }
	
	SocketSet SelectSet::SelectOnWrite(SocketSet& socketSet, Timeout& to)
	{
		return LowLevelSelect(socketSet, WRITEFS, to);
	}

    bool SelectSet::SelectOnExcept(Socket& socket, Timeout& to)
    {
        return LowLevelSelect(socket, EXCEPTFS, to);
    }

	SocketSet SelectSet::SelectOnExcept(SocketSet& socketSet, Timeout& to)
	{
		return LowLevelSelect(socketSet, EXCEPTFS, to);
	}

    bool SelectSet::LowLevelSelect(Socket& socket, const SelectType& type, Timeout& to)
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
			throw SocketException("select failed", socket.getDescriptor());
        }
        else if (retval)
        {
            return true;
        }
        // sinon il n'y a pas de données.
        return false;
    }

	SocketSet SelectSet::LowLevelSelect(SocketSet& socketSet, const SelectType& type, Timeout& to)
    {	        
        fd_set fs;
        FD_ZERO(&fs);

		SocketHandleImpl maxSocket = 0;
		for(SocketSet::size_type index = 0; index != socketSet.size(); index++)
		{
			const SocketHandleImpl& descriptor = socketSet[index]->getDescriptor();
			if(descriptor > maxSocket)
			{
				maxSocket = descriptor;
			}
			FD_SET(descriptor, &fs);
		}

        int retval = 0;
        switch(type)
        {
            case READFS:
                retval = select(maxSocket+1, &fs, 0, 0, &to.c_struct());
                break;
            case WRITEFS:
                retval = select(maxSocket+1, 0, &fs, 0, &to.c_struct());
                break;
            case EXCEPTFS:
                retval = select(maxSocket+1, 0, 0, &fs, &to.c_struct());
                break;
        }
        
        if (retval == SOCKET_ERROR)
        {
			throw SocketException("select failed", 0);
        }
        
		SocketSet output;
		for(SocketSet::size_type index = 0; index != socketSet.size(); index++)
		{
			if(FD_ISSET(socketSet[index]->getDescriptor(), &fs))
			{
				output.push_back(socketSet[index]);
			}
		}
		return output;
    }
}

