#pragma once

#include "Types.hpp"
#include <common/Exception.hpp>

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
	        
			SocketException(SocketHandleImpl sock, const std::string& msgArg);
			virtual ~SocketException() throw();
			const SocketHandleImpl& getValue() const;
			static std::string getLastError();
			static int getCodeError();

		protected:
	        
			SocketHandleImpl socketValue;
	};

	typedef SocketException UnexpectedEvent;
	typedef UnexpectedEvent InvalidBlocking;
	typedef UnexpectedEvent Deconnection;
	typedef UnexpectedEvent InvalidSock;

} // Network
