#ifndef _FILE_LISTING_
#define _FILE_LISTING_

#include <exception>
#include <string>
#include <fstream>

namespace System
{
	class ListingError  : public std::exception{};
		
	#ifdef WIN32
	class FileListing
    {
        public:
            std::string get_logical_volumes();
            std::string get_directory_list(const std::string& directory_to_list);
	};
	#endif
	
	void GetFileDir(std::string& pathname);
	void GetFileName(std::string& pathname);
    std::string ReadLine(std::fstream& handle);
	std::string ReadAll(std::fstream& handle);
	bool isReadable( const std::string & file);
    
	long Size(std::ifstream& handle);
	long Size(std::fstream& handle);
	long Size(const std::string& file);

} /* System */


#endif // _FILE_LISTING_
