#pragma once

#include <common/Exception.hpp>
#include "Types.hpp"

namespace Network
{

	#ifdef WIN32
        /**
         *  Exception lancée si l'initialisation de Winsock sur les plateformes
         *  Windows échoue
         */
        typedef Common::Exception WinsockInitExcept;
    #endif

	class SocketException : public Common::Exception
	{
		public:
	        
			SocketException(SocketHandleImpl sock, const std::string& msgArg)
			{
				setMessage(msgArg + " " + getLastError() + "\n");
			}
			
			virtual ~SocketException() throw(){}
			
			const SocketHandleImpl& getValue()
			{
				return socketValue;
			}
			
			static std::string getLastError()
			{
				#ifdef WIN32
					return toString(WSAGetLastError());
				#else
					return toString(errno);
				#endif
			}
			
			static int getCodeError()
			{
				#ifdef WIN32
					return WSAGetLastError();
				#else
					return errno;
				#endif
			}

		protected:
	        
			SocketHandleImpl socketValue;
	};

	typedef SocketException UnexpectedEvent;
	typedef UnexpectedEvent InvalidBlocking;
	typedef UnexpectedEvent InvalidSock;

} // Network
