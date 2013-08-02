#include <blaspheme/protocol/Session.hpp>
#include <network/TcpServer.hpp>
#include <Innocence.hpp>
using namespace Innocence;

#include <common/Log.hpp>
#include <string>
using std::string;

namespace Blaspheme
{
	SessionId Session::maxIdAttributed = 0;
	
	Session::Session(ConnectionInfo infoArg, Network::TcpClient mainStreamArg)
		: info(infoArg), mainStream(mainStreamArg), sessionId(0)
	{
		LOG << "Session constructor";
		authPlugin = new StringBasedAuth(info);
	}

	Session::Session(const Session& session)
	{
		LOG << "Session copy";
		authPlugin = new StringBasedAuth(session.info);
		info = session.info;
		lastCmdStatus = session.lastCmdStatus;
		maxIdAttributed = session.maxIdAttributed;
		mainStream = session.mainStream;
		sessionId = session.sessionId;
		auxStreams = session.auxStreams;
	}

	Session::~Session()
	{
		LOG << "Session destructor";
		delete authPlugin;
		authPlugin = 0;
	}
	
	bool Session::connect()
	{
		LOG_THIS_FUNCTION
		if(mainStream.connect(info.ip, info.port))
        {
			LOG << "Waiting authentication";
            if(authPlugin->recvAuth(*this))
            {
                *this << "0";
                string stringId;
                SessionId id;
                *this >> stringId;
                fromString(stringId, id);
                sessionId = id;
                
                LOG << "Main connection acquired, my ID : " + stringId;
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

	bool Session::waitConnect()
	{
		LOG_THIS_FUNCTION
		Network::TcpServer listener(info.port);
		if(listener.accept(mainStream, info.deadline))
		{
			LOG << "Client accepted, stop listening";
			listener.stopListen();
			LOG << "Sending authentication";
			if(authPlugin->sendAuth(*this))
            {
				string stringId;
				*this >> stringId;
				fromString(stringId, sessionId);
				LOG << "Stream ID received : " + stringId;
                if(sessionId == 0)
                {
					LOG << "New client connected, sending initial id";
					*this << "0";
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

	const ConnectionInfo& Session::getConnection() const
	{
		return info;
	}

	void Session::setConnection(const ConnectionInfo& infoArg)
	{
		info = infoArg;
	}

} // Blaspheme
