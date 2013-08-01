#include <blaspheme/protocol/Session.hpp>
#include <network/TcpServer.hpp>
#include <blaspheme/Blaspheme.hpp>
#include <common/Log.hpp>
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
		//LOG << Connecting to "+toString(cinfo.ip)+" on port "+toString(cinfo.port);
		if(mainStream.connect(cinfo.ip, cinfo.port))
        {
			authPlugin.setPassword(cinfo.password);
            if(authPlugin.recvAuth(*this))
            {
                send(toString(0));
                string str_id;
                SessionId id;
                recv(str_id);
                fromString(str_id, id);
                sessionId = id;
                
                LOG << "Main connection acquired, my ID : " + str_id;
                return true;
            }
            else
            {
				LOG << "Authentication failed";
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
				fromString(stringId, sessionId);
				LOG << "Stream ID received : " + stringId;
                if(sessionId == 0)
                {
					LOG << "New client connected";
					send("0");
                }
                else
                {
					LOG << "New secondary connection";
                }
				return true;
			}
			else
			{
				LOG << "Authentication failed";
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
        LOG << "Session is resetting";
        // reset des connexions auxiliaires
        while(auxStreams.size())
        {
            Network::TcpClient front = popStream();
            front.reset();
        }
        
        LOG << "Deconnection done.";
        // reset de la connexion principale
        mainStream.reset();
    }

    Network::TcpClient Session::popStream()
    {
        if(!auxStreams.size())
        {
            LOG << "No secondary connection left";
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
