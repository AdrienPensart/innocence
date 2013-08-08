#pragma once

#include <string>
#include "Md5.hpp"

namespace Blaspheme
{
    class Hash
    {
	    private:

		    MD5 md5;
            std::string hashit(const char * chunk, size_t size);
		    std::string convert(unsigned char *bytes);

	    public:

		    std::string getHash(std::string text);
		    std::string getHashFromFile(const std::string& filename);
            std::string getHash(const char * chunk, size_t size);
    };
} // Blaspheme
