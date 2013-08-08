#include "BinaryRessource.hpp"

#include <log/Log.hpp>
#include <common/Exception.hpp>
#include <Windows.h>

namespace Malicious
{
	BinaryRessource::BinaryRessource(const unsigned char * ressourceName, std::size_t size, const std::string& _ressourceFile, const bool& _deleteIt)
		:ressourceFile(_ressourceFile), deleteIt(_deleteIt)
	{
	    deleteRessource();
		LOG << "Writing binary ressource " + ressourceFile + " of size " + Common::toString(size);
	    HANDLE hFile = CreateFile(ressourceFile.c_str(),GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			throw Common::Exception("CreateFile failed : " + Common::toString(GetLastError()));
		}
		
	    DWORD wmWritten = 0; 
	    if(!WriteFile(hFile, ressourceName,(DWORD)size, &wmWritten, NULL))
		{
			throw Common::Exception("WriteFile failed : " + Common::toString(GetLastError()));
		}

	    if(wmWritten != size)
	    {
			Common::Exception writingError("Incomplete writing of file " + ressourceFile + " : " + Common::toString(GetLastError()));
		    throw writingError;
	    }

		CloseHandle(hFile);
    }

    BinaryRessource::~BinaryRessource()
    {
	    if(deleteIt)
	    {
		    deleteRessource();
	    }
    }

	void BinaryRessource::deleteRessource()
    {
	    DWORD attrib = GetFileAttributes(ressourceFile.c_str());
	    if(attrib != INVALID_FILE_ATTRIBUTES)
	    {
		    if(!DeleteFile(ressourceFile.c_str()))
			{
				throw Common::Exception("Unable to delete file : " + ressourceFile + " : " + Common::toString(GetLastError()));
			}
			LOG << "Deleting binary ressource " + ressourceFile;
	    }
    }
} // Malicious
