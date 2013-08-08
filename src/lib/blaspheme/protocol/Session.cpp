#include <blaspheme/protocol/Session.hpp>
#include <network/TcpServer.hpp>
#include <common/Innocence.hpp>

#include <log/Log.hpp>
#include <string>
using std::string;

namespace Blaspheme
{
	SessionId Session::maxIdAttributed = 0;
	
	Session::Session(Common::ConnectionInfo infoArg, Network::Timeout deadlineArg) :
		info(infoArg),
		sessionId(0),
		auth(0),
		cipher(0),
		deadline(deadlineArg)
	{
		LOG << "Session constructor";
		auth = new StringBasedAuth(info);
		cipher = new NoCiphering;
	}

	Session::Session(const Session& session)
	{
		LOG << "Session copy";
		cipher = new NoCiphering;
		auth = new StringBasedAuth(session.info);
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
		delete auth;
		auth = 0;
		delete cipher;
		cipher = 0;
	}
	
	bool Session::connect()
	{
		TRACE_FUNCTION
		if(mainStream.connect(info.ip, info.port))
        {
			LOG << "Waiting authentication";
            if(auth->recvAuth(*this))
            {
                *this << "0";
                string stringId;
                SessionId id;
                *this >> stringId;
                Common::fromString(stringId, id);
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
		TRACE_FUNCTION
		Network::TcpServer listener(info.port);
		if(listener.accept(mainStream, deadline))
		{
			LOG << "Client accepted, stop listening";
			listener.stopListen();
			LOG << "Sending authentication";
			if(auth->sendAuth(*this))
            {
				string stringId;
				*this >> stringId;
				Common::fromString(stringId, sessionId);
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
		std::string buffer = object+'\n';
		cipher->encrypt(buffer);
        mainStream.send(buffer);
        return *this;
    }

	Session& Session::operator>>(std::string& object)
    {
		std::string buffer;
        mainStream.recv(buffer, '\n', false);
		cipher->decrypt(buffer);
		object = buffer;
        return *this;
    }

	Session& Session::send(const std::string& object)
    {
		std::string buffer = object+'\n';
		cipher->encrypt(buffer);
        mainStream.send(buffer);
        return *this;
    }

	Session& Session::recv(std::string& object)
    {
		std::string buffer;
        mainStream.recv(object, '\n', false);
		cipher->decrypt(buffer);
		object = buffer;
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

	const Common::ConnectionInfo& Session::getConnection() const
	{
		return info;
	}

	void Session::setConnection(const Common::ConnectionInfo& infoArg)
	{
		info = infoArg;
	}

} // Blaspheme
