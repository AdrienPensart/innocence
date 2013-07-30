#ifndef _CONNECTION_INFO_
#define _CONNECTION_INFO_

#include <network/Types.hpp>
#include <network/Timeout.hpp>

#define MARKER_SIZE 3
#define MARKER "*#+"
#define SEPERATOR ':'

namespace Blaspheme
{
    enum {DEFAULT_STR_SIZE = 256};
    
    struct ConnectionInfo
    {
        std::string name;
        std::string ip;
        Network::Port port;
        std::string password;
		Network::Timeout deadline;
    };
    
} /* Blaspheme */

#endif // _CONNECTION_INFO_
