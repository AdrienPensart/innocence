#include <log/Log.hpp>
#include <blaspheme/protocol/Authentication.hpp>
using namespace Network;

#include "Session.hpp"

namespace Blaspheme {
	AuthenticationMethod::AuthenticationMethod(const Common::ConnectionInfo& infoArg) :
		info(infoArg) {
	}

	const Common::ConnectionInfo& AuthenticationMethod::getInfo() {
		return info;
	}

	bool NoAuthentication::sendAuth(Session&) {
		TRACE_FUNCTION
		LOG << "No authentication plugin";
		return true;
	}

	bool NoAuthentication::recvAuth(Session&) {
		TRACE_FUNCTION
		LOG << "No authentication plugin";
		return true;
	}

	StringBasedAuth::StringBasedAuth(const Common::ConnectionInfo& infoArg) :
		AuthenticationMethod(infoArg) {
	}

	bool StringBasedAuth::sendAuth(Session& session) {
		TRACE_FUNCTION
		LOG << "Sending password";
		session << getInfo().password;
		std::string buffer;
		session >> buffer;
		if(buffer == SUCCESS) {
			return true;
		} else if(buffer == FAILURE) {
			LOG << "Authentication failed";
		} else {
			LOG << "Authentication bad response";
		}
		return false;
	}

	bool StringBasedAuth::recvAuth(Session& session) {
		TRACE_FUNCTION
		LOG << "Waiting password";
		std::string received_password;
		session >> received_password;
		LOG << "Password received : " + received_password;
		if(received_password == getInfo().password) {
			session << SUCCESS;
			return true;
		}
		session << FAILURE;
		return false;
	}

	ChallengedBasedAuth::ChallengedBasedAuth(const Common::ConnectionInfo& infoArg) :
		AuthenticationMethod(infoArg) {
	}

	bool ChallengedBasedAuth::sendAuth(Session&) {
		TRACE_FUNCTION
		LOG << "Not implemented";
		return false;
	}

	bool ChallengedBasedAuth::recvAuth(Session&) {
		TRACE_FUNCTION
		LOG << "Not implemented";
		return false;
	}

} // Blaspheme
