#ifndef _BINARY_RESSOURCE_HPP
#define _BINARY_RESSOURCE_HPP

#include <cstdio>
#include <string>
#include <common/Logger.hpp>

namespace Malicious
{

    class BinaryRessource
    {
	    public:

		    BinaryRessource(const unsigned char * ressourceName, std::size_t size, const std::string& _ressourceFile, const bool& _deleteIt=false);
		    ~BinaryRessource();

	    private:
    		
		    bool deleteRessource();
		    std::string ressourceFile;
		    bool deleteIt;
    };

} // Malicious

#endif // _BINARY_RESSOURCE_HPP
