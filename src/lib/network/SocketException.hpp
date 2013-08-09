#pragma once

#include "Types.hpp"
#include <common/Exception.hpp>

namespace Network
{
	class SocketException : public Common::Exception
	{
		public:
	        
			SocketException(const std::string& msgArg, SocketHandleImpl sock=0, int errorCode=0);
			virtual ~SocketException() throw();
			const SocketHandleImpl& getValue() const;
			static std::string getLastError();
			static int getCodeError();

		protected:
	        
			int errorCode;
			SocketHandleImpl socketValue;
	};

	#ifdef WIN32
        /**
         *  Exception lancée si l'initialisation de Winsock sur les plateformes
         *  Windows échoue
         */
        typedef SocketException WinsockInitExcept;
    #endif

	typedef SocketException InvalidBlocking;
	typedef SocketException Deconnection;
	typedef SocketException InvalidSock;

} // Network
