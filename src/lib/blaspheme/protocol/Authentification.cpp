#include <common/Log.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <blaspheme/protocol/Authentication.hpp>
using namespace Network;

#include "Session.hpp"

namespace Blaspheme
{
	AuthenticationMethod::AuthenticationMethod(const ConnectionInfo& infoArg) :
		info(infoArg)
	{
	}

	const ConnectionInfo& AuthenticationMethod::getInfo()
	{
		return info;
	}

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

    StringBasedAuth::StringBasedAuth(const ConnectionInfo& infoArg) : 
		AuthenticationMethod(infoArg)
    {
    }

    bool StringBasedAuth::sendAuth(Session& session)
    {
		FUNC_LOG(__FUNCTION__);
        LOG << "Sending password";
        session << getInfo().password;
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
        if(received_password == getInfo().password)
        {
            session << SUCCESS;
            return true;
        }
        session << FAILURE;
        return false;
    }

	ChallengedBasedAuth::ChallengedBasedAuth(const ConnectionInfo& infoArg) : 
		AuthenticationMethod(infoArg)
	{
	}

    bool ChallengedBasedAuth::sendAuth(Session&)
	{
		FUNC_LOG(__FUNCTION__);
		LOG << "Not implemented";
		return false;
	}

	bool ChallengedBasedAuth::recvAuth(Session&)
	{
		FUNC_LOG(__FUNCTION__);
		LOG << "Not implemented";
		return false;
	}

} // Blaspheme
