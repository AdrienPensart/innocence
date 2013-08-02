#pragma once

#include "Convert.hpp"
#include <string>
#include <exception>

namespace Common
{
    class Exception : public std::exception
    {
		public:

			Exception(const std::string msg="Generic Common Exception");
			virtual ~Exception() throw();
			virtual const char * what() const throw ();
			
        protected:

			void setMessage(const std::string msg);

		private:

			std::string msg;
    };

} // Common

#pragma once