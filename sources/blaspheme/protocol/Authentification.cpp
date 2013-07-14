#include <common/Logger.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <blaspheme/protocol/Authentification.hpp>
using namespace Network;

#include "Session.hpp"

namespace Blaspheme
{
    StringBasedAuth::StringBasedAuth(const std::string& string_password)
    :password(string_password)
    {
    }

    void StringBasedAuth::setPassword(const std::string& string_password)
    {
        password = string_password;
    }

    bool StringBasedAuth::sendAuth(Session& session)
    {
        LOG << "Authentification en cours...";
        std::string received_password;
        session >> received_password;
        LOG << "Mot de passe recu : " + received_password;
        if(received_password == password)
        {
            session << SUCCESS;
            return true;
        }
        session << FAILURE;
        return false;
    }

    bool StringBasedAuth::recvAuth(Session& session)
    {
        LOG << "Authentification en cours avec : " + password;
        session << password;        
        std::string buffer;
        session >> buffer;
        if(buffer == SUCCESS)
        {
            return true;
        }
        else if(buffer == FAILURE)
        {
            LOG << "Echec de l'authentification.";
        }
        else
        {
            LOG << "Probleme : reponse d'authentification.";           
        }
        return false;
    }

} /* Blaspheme */
