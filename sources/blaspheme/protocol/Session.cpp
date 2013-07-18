#include <blaspheme/protocol/Session.hpp>
#include <network/TcpServer.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <common/Logger.hpp>
#include <string>
using std::string;

namespace Blaspheme
{
	SessionId Session::maxIdAttributed = 0;
	
	Session::Session(Network::TcpClient _mainStream)
		: mainStream(_mainStream), sessionId(0)
	{
	}

	Session::~Session()
	{
		//LOG << "Session destructor";
		//delete authPlugin;
	}
	
	bool Session::connect(Blaspheme::ConnectionInfo& cinfo)
	{
		//LOG << "Try connect to "+to_string(cinfo.ip)+" on port "+to_string(cinfo.port);
		if(mainStream.connect(cinfo.ip, cinfo.port))
        {
			authPlugin.setPassword(cinfo.password);
            if(authPlugin.recvAuth(*this))
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
		//LOG << "Listening for incoming connections...";
		if(listener.accept(mainStream, cinfo.deadline))
		{
			LOG << "Client accepted.";
			listener.stopListen();
			authPlugin.setPassword(cinfo.password);
			if(authPlugin.sendAuth(*this))
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
    /*
	void Session::setAuthentication(AuthenticationMethod * _authPlugin)
	{
		delete authPlugin;
		authPlugin = _authPlugin;
	}
	*/
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

	const SessionId& Session::getId()
	{
		return sessionId;
	}

    void Session::setId(const SessionId& newId)
	{
		sessionId = newId;
	}

    SessionId Session::getNextId()
	{
		return (++maxIdAttributed);
	}

	Network::TcpClient& Session::stream()
	{
		return mainStream;
	}

} /* Blaspheme */
