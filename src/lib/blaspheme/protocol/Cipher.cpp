#include "Cipher.hpp"

namespace Blaspheme
{
	NoCiphering::NoCiphering()
	{
	}

    void NoCiphering::encrypt(std::string&)
	{
	}

    void NoCiphering::decrypt(std::string&)
	{
	}

    XORCiphering::XORCiphering(const std::string& initial_key)
    :xor_key(initial_key)
    {

    }

    void XORCiphering::encrypt(std::string& object)
    {
        x_or(object);
    }

    void XORCiphering::decrypt(std::string& object)
    {
        x_or(object);
    }

    void XORCiphering::x_or(std::string& chaine)
    {
        unsigned int index_chaine = 0;
        unsigned int index_cle = 0;
        for(;
            index_chaine != chaine.size();
            ++index_chaine, ++index_cle
           )
        {
            if(index_cle == xor_key.size())
            {
                index_cle = 0;
            }
            chaine[index_chaine] ^= xor_key[index_cle];
        }
    }

    void XORCiphering::setKey(const std::string& gived_xor_key)
    {
        xor_key = gived_xor_key;
    }

} // Blaspheme
