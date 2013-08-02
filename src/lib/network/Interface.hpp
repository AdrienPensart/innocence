#pragma once

#include "Types.hpp"

namespace Network
{
    namespace BSDSocket
    {
		inline int bsd_close(int sockfd)
		{
			return closesocket(sockfd);
		}
		
        inline int bsd_connect(int sockfd, sockaddr *serv_addr, socklen_t addrlen)
        {
            return connect(sockfd,serv_addr,addrlen);
        }
        
        inline int bsd_listen(int s, int backlog)
        {
            return listen(s, backlog);
        }
        
        inline int bsd_accept(int sock, sockaddr *adress, socklen_t *size)
		{
            return accept(sock, adress, size);
        }
        
        inline int bsd_send(int s, const char * msg, size_t len, int flags)
        {
            return send(s, msg, len, flags);
        }
        
        inline int bsd_recv(int s, char * buf, int len, unsigned int flags)
        {
            return recv(s, buf, len, flags);
        }
    }
} // Network
