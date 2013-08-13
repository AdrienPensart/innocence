#include "LogServer.hpp"
#include "Log.hpp"
#include <common/Identity.hpp>
using namespace Network;

namespace Log
{
	LogServer::LogServer(const Port& port, Timeout deadlineArg) : 
		deadline(deadlineArg),
		interrupt(false)
	{
		server = new TcpServer(port);
		in.push_back(server);
	}

	void LogServer::run()
	{
		try
		{
			while(!interrupt)
			{
				try
				{
					SocketSet out = SelectSet::SelectOnRead(in, deadline);
					for(size_t index = 0; index < out.size(); index++)
					{
						if(out[index] == server)
						{
							TcpClient * client = new TcpClient();
							server->accept(*client);
							in.push_back(client);
						}
						else
						{
							std::string buffer;
							std::string graph;
							TcpClient * client = (TcpClient*)out[index];
							client->recv(buffer, '\n', deadline);
							if(buffer.size())
							{
								Message message;
								message.deserialize(buffer);
								message.getIdentity().setIp(client->getIp());
								notify(message);
							}
						}
					}
				}
				catch(Deconnection& d)
				{
					for(size_t index = 0; index < in.size(); index++)
					{
						if(d.getValue() == in[index]->getDescriptor())
						{
							delete in[index];
							in.erase(in.begin() + index);
							break;
						}
					}
				}
			}

			for(size_t index = 0; index < in.size(); index++)
			{
				delete in[index];
			}
		}
		catch(Common::Exception&)
		{
			LOG << "LogServer exception trap";
		}
		CATCH_UNKNOWN_EXCEPTION
	}

	void LogServer::stop()
	{
		interrupt = true;
	}

} // Log
