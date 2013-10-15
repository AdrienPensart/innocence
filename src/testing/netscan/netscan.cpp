#include <log/Log.hpp>
#include <common/Innocence.hpp>
#include <common/ParseOptions.hpp>
#include <audit/Audit.hpp>
#include <malicious/NetScan.hpp>

int submain(int argc, char ** argv)
{
	try
	{
		LOG.setIdentity(Common::identity);
		Common::ParseOptions(argc, argv);

		Malicious::HostList hostList;
		Malicious::PortList portList;

		hostList.push_back("192.168.0.1");
		hostList.push_back("127.0.0.1");
		hostList.push_back("192.168.0.2");
		portList.push_back(80);
		portList.push_back(81);
		portList.push_back(82);
		
		Malicious::NetScan ns(hostList, portList, true);
		ns.start();
		ns.join();
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN