#ifndef _MALICIOUS_EXCEPTION_
#define _MALICIOUS_EXCEPTION_

#include <exception>
#include <string>

namespace Malicious
{

class Exception : public std::exception
{
	public:

		explicit Exception(const std::string& argMsgError);
		virtual ~Exception() throw();
		virtual const char * what() const throw ();

	private:

		std::string msgError;
};

} // Malicious

#endif // _MALICIOUS_EXCEPTION_
