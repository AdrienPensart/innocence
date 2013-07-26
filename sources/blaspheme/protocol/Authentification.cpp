#include <common/Log.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <blaspheme/protocol/Authentication.hpp>
using namespace Network;

#include "Session.hpp"

namespace Blaspheme
{
	bool NoAuthentication::sendAuth(Session&)
	{
		FUNC_LOG(__FUNCTION__);
		LOG << "No authentication plugin";
		return true;
	}

	bool NoAuthentication::recvAuth(Session&)
	{
		FUNC_LOG(__FUNCTION__);
		LOG << "No authentication plugin";
		return true;
	}

    StringBasedAuth::StringBasedAuth(const std::string& string_password)
    :password(string_password)
    {
    }

    void StringBasedAuth::setPassword(const std::string& string_password)
    {
        password = string_password;
    }

    bool StringBasedAuth::sendAuth(Session& session)
    {
		FUNC_LOG(__FUNCTION__);
        LOG << "Sending password";
        session << password;
        std::string buffer;
        session >> buffer;
        if(buffer == SUCCESS)
        {
            return true;
        }
        else if(buffer == FAILURE)
        {
            LOG << "Authentication failed";
        }
        else
        {
            LOG << "Authentication bad response";
        }
        return false;
    }

    bool StringBasedAuth::recvAuth(Session& session)
    {
		FUNC_LOG(__FUNCTION__);
		LOG << "Waiting password";
        std::string received_password;
        session >> received_password;
        LOG << "Password received : " + received_password;
        if(received_password == password)
        {
            session << SUCCESS;
            return true;
        }
        session << FAILURE;
        return false;
    }

} /* Blaspheme */
