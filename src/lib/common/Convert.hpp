#pragma once

#include <sstream> 
#include <string>

template<typename T>
std::string toString( const T & Value )
{
    // utiliser un flux de sortie pour cr�er la cha�ne
    std::ostringstream oss;
    // �crire la valeur dans le flux
    oss << Value;
    // renvoyer une string
    return oss.str();
}

template<typename T>
bool fromString( const std::string & Str, T & Dest )
{
    // cr�er un flux � partir de la cha�ne donn�e
    std::istringstream iss( Str );
    // tenter la conversion vers Dest
    return iss >> Dest != 0;
}
