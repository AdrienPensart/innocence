#pragma once

#include <system/Thread.hpp>
#include <network/Network.hpp>
#include <vector>
#include <map>

namespace Malicious {
	typedef std::vector<Network::Host> HostList;
	typedef std::vector<Network::Port> PortList;

	class NetScan :
		public System::Thread {
		public:
			NetScan(const HostList& hosts, const PortList& ports, bool randomize=false, Network::Timeout delay=Network::Timeout(1));
			virtual void start();
			virtual void stop();

		private:
			bool randomize;
			Network::Timeout delay;
			HostList hosts;
			PortList ports;
			std::map<Network::Host, std::map<Network::Port, bool> > results;
			bool scan(const Network::Host& host, const Network::Port& port);
			static DWORD WINAPI ScanLoop(LPVOID parameter);
	};

} // Malicious
