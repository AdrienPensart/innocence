#ifndef _BINARY_RESSOURCE_
#define _BINARY_RESSOURCE_

#include <cstdio>
#include <string>

namespace Malicious
{

    class BinaryRessource
    {
	    public:

		    BinaryRessource(const unsigned char * ressourceName, std::size_t size, const std::string& _ressourceFile, const bool& _deleteIt=false);
		    ~BinaryRessource();

	    private:
    		
		    void deleteRessource();
		    std::string ressourceFile;
		    bool deleteIt;
    };

} // Malicious

#endif // _BINARY_RESSOURCE_
