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
				if(index+1 < argc)
				{
					index++;
					std::vector<std::string> results;
					split(argv[index], ":", results);
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
				}
				else
				{
					throw ParseError("Value missing");
				}
			}
			else if(arg == "-collector")
			{
				if(index+1 < argc)
				{
					index++;
					std::vector<std::string> results;
					split(argv[index], ":", results);
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
				}
				else
				{
					throw ParseError("Value missing");
				}
			}
			else if(arg == "-file")
			{
				if(index+1 < argc)
				{
					std::string next = argv[index+1];
					if(next == "-console" || next == "-collector" || next == "-auditor")
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
			else
			{
				throw ParseError("Bad argument");
			}
		}
	}
} // Common
