//!
//!     Kaleidoscalp, all rights reserved.
//!

#include <string>
using namespace std;

#include "ServerWindow.hpp"
#include "ListeningThread.hpp"

#include <blaspheme/protocol/Session.hpp>
#include <common/Log.hpp>
#include <network/TcpClient.hpp>
using namespace Network;
using namespace Blaspheme;

namespace TheSleeper
{
	ListeningThread::ListeningThread(int port_value, const QString& pass_value, QObject * parent)
		:QThread(parent), continue_listen(false), session(0)
	{
		changeListeningPort(port_value);
		changePassword(pass_value);
		info.deadline.set(0, 200);
	}

    ListeningThread::~ListeningThread()
	{
		stopListen();
	}

    void ListeningThread::run()
    {
        LOG << "Debut Thread.";
        try
        {
            LOG << "Boucle de connexion active.";
            while(continue_listen)
            {
				Session session;
				if(session.wait_connect(info))
				{
					LOG << "ListeningThread::run() : nouveau client.";
					emit newClientConnected(session);
				}
            }
			LOG << "Fin boucle de connexion.";
        }
		catch(Network::Deconnection&)
		{
			LOG << "ListeningThread::run() : Deconnexion intempestive.";
		}
        catch(Network::Exception& e)
        {
			LOG << "network exception";
            LOG << e.what();
        }
        catch(std::exception& e)
        {
			LOG << "standard exception";
            LOG << e.what();
        }
        catch(...)
        {
            LOG << "Erreur dans le thread de boucle des connexions.";
        }
        LOG << "Fin thread.";
    }

    bool ListeningThread::isListening()
    {
        return continue_listen;
    }

    void ListeningThread::changeListeningPort(int port)
    {
        LOG << "Mise a jour du port d'ecoute : " + to_string(port);
        info.port = port;
    }

    void ListeningThread::changePassword(const QString& password)
    {
		info.password = password.toStdString();
		LOG << "Mise a jour du mot de passe de connexion : " + info.password;
    }

    void ListeningThread::setListening(bool enabled)
    {
        enabled ? listen() : stopListen();
    }

    void ListeningThread::setNotListening(bool enabled)
    {
        enabled ? stopListen() : listen();
    }

    void ListeningThread::listen()
    {
		if(!continue_listen)
		{
			continue_listen = true;
			start();
		}
    }

    void ListeningThread::stopListen()
    {
		if(continue_listen)
		{
			continue_listen = false;
			wait();
		}
	}
    
} // TheSleeper

