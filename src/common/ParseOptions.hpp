#pragma once

#include <string>
#include <iostream>

#include <audit/Audit.hpp>
#include <network/Types.hpp>
#include <log/Log.hpp>

#include "Build.hpp"
#include "Utility.hpp"
#include "Exception.hpp"

namespace Common
{
	typedef Exception ParseError;

	void ParseOptions(int argc, char ** argv)
	{
		for(int index = 1; index < argc; index++)
		{
			std::string arg = argv[index];
			if(arg == "-console")
			{
				LOG.addObserver(new Log::LogToConsole);
			}
			else if(arg == "-auditor")
			{
				if(index+1 > argc)
				{
					throw ParseError("Value missing");
				}

				std::string next = argv[index+1];
				if(next == "-console" || next == "-auditor" || next == "-collector" || next == "-sql" || next == "-file")
				{
					throw ParseError("Value missing");
				}

				std::vector<std::string> results;
				split(next, ":", results);
				if(results.size() != 2)
				{
					throw ParseError("Bad argument to auditor option");
				}

				std::string ip = results[0];
				std::string portStr = results[1];
				Network::Port port;
				if(!fromString(portStr, port))
				{
					throw ParseError("Bad port format");
				}

				LOG.addObserver(new Audit::LogToAuditor(ip, port));
				index++;
			}
			else if(arg == "-collector")
			{
				if(index+1 > argc)
				{
					throw ParseError("Value missing");
				}

				std::string next = argv[index+1];
				if(next == "-console" || next == "-auditor" || next == "-collector" || next == "-sql" || next == "-file")
				{
					throw ParseError("Value missing");
				}

				std::vector<std::string> results;
				split(next, ":", results);
				if(results.size() != 2)
				{
					throw ParseError("Bad argument to auditor option");
				}

				std::string ip = results[0];
				std::string portStr = results[1];
				Network::Port port;
				if(!fromString(portStr, port))
				{
					throw ParseError("Bad port format");
				}

				LOG.addObserver(new Log::LogToCollector(ip, port));
				index++;
			}
			else if(arg == "-file")
			{
				if(index+1 < argc)
				{
					std::string next = argv[index+1];
					if(next == "-console" || next == "-auditor" || next == "-collector" || next == "-sql" || next == "-file")
					{
						LOG.addObserver(new Log::LogToFile(Common::identity.getLogFile()));
					}
					else
					{
						LOG.addObserver(new Log::LogToFile(next));
						index++;
					}
				}
				else
				{
					LOG.addObserver(new Log::LogToFile(Common::identity.getLogFile()));
				}
			}
			#ifdef WIN32
			else if(arg == "-sql")
			{
				if(index+1 > argc)
				{
					throw ParseError("Connection string missing");
				}

				std::string next = argv[index+1];
				if(next == "-console" || next == "-auditor" || next == "-collector" || next == "-sql" || next == "-file")
				{
					throw ParseError("Connection string missing");
				}
				LOG.addObserver(new Log::LogToSql(next));
				index++;
			}
			#endif
			else
			{
				throw ParseError("Bad argument");
			}
		}
	}
} // Common
