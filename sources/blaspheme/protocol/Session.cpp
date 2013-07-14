#include <blaspheme/protocol/Session.hpp>
#include <network/TcpServer.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <common/Logger.hpp>
#include <string>
using std::string;

namespace Blaspheme
{
	SessionId Session::maxIdAttributed = 0;
	
	Session::Session(AuthentificationMethod * _authPlugin, Network::TcpClient _mainStream)
		: mainStream(_mainStream), authPlugin(_authPlugin), sessionId(0)
	{
	}

	
	Session::~Session()
	{
		delete authPlugin;
	}
	
	bool Session::connect(Blaspheme::ConnectionInfo& cinfo)
	{
		if(mainStream.connect(cinfo.ip, cinfo.port))
        {
            if(authPlugin->recvAuth(*this))
            {
                send(to_string(0));
                string str_id;
                SessionId id;
                recv(str_id);
                from_string(str_id, id);
                sessionId = id;
                
                LOG << "ID attribue : " + str_id;
                LOG << "Connexion principale acquise.";
                return true;
            }
            else
            {
				LOG << "Echec de l'authentification.";
                reset();
            }
        }
		return false;
	}
	bool Session::wait_connect(Blaspheme::ConnectionInfo& cinfo)
	{
		Network::TcpServer listener(cinfo.port);
		if(listener.accept(mainStream, cinfo.deadline))
		{
			listener.stopListen();
			if(authPlugin->sendAuth(*this))
            {
				string stringId;
				recv(stringId);
				from_string(stringId, sessionId);
				LOG << "Stream ID recu : " + stringId;
                if(sessionId == 0)
                {
					LOG << "Nouveau client connecte.";
					send("0");
                }
                else
                {
					LOG << "Nouvelle connexion auxiliaire.";
                }
				return true;
			}
			else
			{
				LOG << "Echec de l'authentification.";
			}
		}
		return false;
	}
    
	void Session::setAuthenfication(AuthentificationMethod * _authPlugin)
	{
		delete authPlugin;
		authPlugin = _authPlugin;
	}
	
	Session& Session::operator<<(const std::string& object)
    {
        mainStream.send(object+'\n');
        return *this;
    }
	Session& Session::operator>>(std::string& object)
    {
        mainStream.recv(object, '\n', false);
        return *this;
    }
	Session& Session::send(const std::string& object)
    {
        mainStream.send(object+'\n');
        return *this;
    }
	Session& Session::recv(std::string& object)
    {
        mainStream.recv(object, '\n', false);
        return *this;
    }  
    void Session::reset()
    {
        LOG << "Reset de toutes les connexions...";
        // reset des connexions auxiliaires
        while(auxStreams.size())
        {
            Network::TcpClient front = popStream();
            front.reset();
        }
        
        LOG << "Deconnection : OK.";
        // reset de la connexion principale
        mainStream.reset();
    }

    Network::TcpClient Session::popStream()
    {
        if(!auxStreams.size())
        {
            LOG << "Pas d'objet dans la stack.";
            return Network::TcpClient();
        }
        Network::TcpClient front = auxStreams.top();
        auxStreams.pop();
        return front;
    }
	void Session::pushStream(Network::TcpClient& stream)
	{
		auxStreams.push(stream);
	}

} /* Blaspheme */
