#include "LogObserver.hpp"
#include "Log.hpp"
#include "Message.hpp"

#include <common/Settings.hpp>
#include <iostream>

std::string GetFileName(const std::string& pathname)
{
	size_t last = pathname.find_last_of('\\');
	return pathname.substr(last+1);
}

namespace Log
{
	LogObserver::~LogObserver()
	{

	}

	LogToNetwork::LogToNetwork(const Network::Host& loggerIp, const Network::Port& loggerPort)
	{
		//socket.setDestInfo(loggerIp, loggerPort);
		connected = socket.connect(loggerIp, loggerPort);
		if(!connected)
		{
			LOG << "Failed to connect to network observer on " + loggerIp + ":" + Common::toString(loggerPort);
		}
	}

	void LogToNetwork::update(const Message& message)
	{
		try
		{
			if(connected)
			{
				socket << message.serialize() + '\n';
			}
		}
		catch(Network::Deconnection&)
		{
			connected = false;
		}
	}

	Network::Stream&  LogToNetwork::getStream()
	{
		return socket;
	}

	LogToCollector::LogToCollector() :
		LogToNetwork(Common::LOG_COLLECTOR_IP, Common::LOG_COLLECTOR_PORT)
	{
	}

	LogToCollector::LogToCollector(const Network::Host& loggerIp, const Network::Port& loggerPort) :
		LogToNetwork(loggerIp, loggerPort)
	{
	}

	LogToFile::LogToFile(const std::string& filepathArg) :
		filepath(filepathArg),
		disabled(false)
	{
		file.open(filepath.c_str(), std::ofstream::out | std::ofstream::trunc);
		if(!file.is_open())
		{
			disabled = true;
			LOG << "Unable to create or open log file " + filepath;
		}
	}

	void LogToFile::update(const Message& message)
	{
		if(!disabled)
		{
			file  << message.getIdentity().getModule()
				  << " on " << message.getIdentity().getOs()
				  << " -> (" << message.getTime() << ", " << message.getLine()
				  << " in " << GetFileName(message.getFile())
				  << ") (" << message.getCallStack()
				  << ") : "
				  << message.getContent()
				  << '\n';
		}
	}

	LogToConsole::LogToConsole(const std::string& _title)
	:title(_title)
	{
	}

	void LogToConsole::update(const Message& message)
	{
		std::cout << message.getIdentity().getModule()
				  << " on " << message.getIdentity().getOs()
				  << " -> (" << message.getTime() << ", line " << message.getLine()
				  << " in " << GetFileName(message.getFile())
				  << ") (" << message.getCallStack()
				  << ") : "
				  << message.getContent()
				  << '\n';
	}
#ifdef WIN32
	LogToSql::LogToSql(const std::string& connectionStringArg) :
		connectionString(connectionStringArg)
	{
		otl_connect::otl_initialize();
		db.rlogon(connectionString.c_str());

		logdb = new otl_stream(1,
			  "INSERT INTO log (module, buildAt, buildId, callStack, computer, content, emittedAt, executedAt, file, ip, line, system) "
			  "VALUES (:f1<char[255]>,:f2<char[255]>,:f3<char[255]>,:f4<char[255]>,"
						 ":f5<char[255]>,:f6<char[32000]>,:f7<char[255]>,:f8<char[255]>,"
						 ":f9<char[255]>,:f10<char[255]>,:f11<char[255]>,:f12<char[255]>) ",
			  db
			 );
	}

	LogToSql::~LogToSql()
	{
		delete logdb;
		db.logoff();
	}

	void LogToSql::update(const Message& message)
	{
		try
		{
			*logdb << message.getIdentity().getModule().c_str()
				   << message.getIdentity().getBuildDate().c_str()
				   << message.getIdentity().getBuildId().c_str()
				   << message.getCallStack().c_str()
				   << message.getIdentity().getComputer().c_str()
				   << message.getContent().c_str()
				   << message.getTime().c_str()
				   << message.getIdentity().getExecutionDate().c_str()
				   << message.getFile().c_str()
				   << message.getIdentity().getIp().c_str()
				   << message.getLine().c_str()
				   << message.getIdentity().getComputer().c_str();
		}
		catch(otl_exception& p)
		{
			throw Common::Exception("OTL Exception (SQL) : " + Common::toString(p.msg) + ", " + p.stm_text + ", " + p.var_info);
		}
	}
#endif
} // Log
