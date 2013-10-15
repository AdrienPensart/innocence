#pragma once

#include <string>
#include <exception>
#include <typeinfo>
#include "Utility.hpp"

namespace Common
{
    class Exception : public std::exception
    {
		public:

			Exception(const std::string& msgArg="Generic Common Exception") : 
				msg(msgArg)
			{
			}
			
			virtual ~Exception() throw()
			{
			}
			
			virtual const char * what() const throw ()
			{
				return msg.c_str();
			}
			
        protected:

			void setMessage(const std::string& msgArg)
			{
				msg = msgArg;
			}

		private:
			
			std::string msg;
    };
} // Common
