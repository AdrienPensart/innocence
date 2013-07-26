#include "LogServer.hpp"
#include "Log.hpp"

namespace Common
{
	LogServer::LogServer(Network::Port portArg, const std::string& onProofArg)
		:dwThread(EXIT_FAILURE), hThread(0), port(portArg), onProof(onProofArg), proofReceived(false), interrupted(true)
	{
	}

	LogServer::~LogServer()
	{
		stop();
	}

	const std::string& LogServer::getProof()
	{
		return onProof;
	}

	bool LogServer::isProofReceived()
	{
		return proofReceived;
	}

	Network::Port LogServer::getPort()
	{
		return port;
	}

	void LogServer::start()
	{
		interrupted = false;
		hThread = CreateThread(0, 0,(LPTHREAD_START_ROUTINE)MsgLoop, this, 0, &dwThread);
		if(!hThread)
		{
			LOG << "CreateThread failed : " + to_string(GetLastError());
			interrupted = true;
		}
	}

	void LogServer::stop()
	{
		interrupted = true;
		WaitForSingleObject(hThread, INFINITE);
	}

	DWORD WINAPI LogServer::MsgLoop(LPVOID lpParameter)
	{
		try
		{
			LogServer * logServer = (LogServer *)lpParameter;
			Network::UdpSocket server;
			server.listen(logServer->getPort());
			Network::Timeout deadline(0, 100);
			std::string buffer;
			while(!logServer->interrupted)
			{
				server.recv(buffer, deadline);
				if(buffer.size())
				{
					LOG << buffer;
					if(buffer == logServer->getProof())
					{
						logServer->interrupted = true;
						LOG << "Interrupting LogServer Thread";
					}
				}
			}
		}
		catch(std::exception e)
		{
			LOG << e.what();
		}
		catch(...)
		{
			LOG << "Unknow exception from LogServer";
		}
		return EXIT_SUCCESS;
	}

} // Common