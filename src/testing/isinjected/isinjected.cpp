#include <log/Log.hpp>
#include <system/Uac.hpp>
#include <system/Process.hpp>
#include <network/Pipe.hpp>
#include <common/Innocence.hpp>
#include <audit/Audit.hpp>

DWORD WINAPI subdll(void) {
	try {
		LOG.setIdentity(Common::identity);
		LOG.addObserver(new Log::LogToConsole);
		LOG.addObserver(new Log::LogToCollector);
		LOG.addObserver(new Audit::LogToAuditor);

		System::Process::This thisProcess;
		LOG << "DLL getPath : " + thisProcess.getPath();
		LOG << "Current process name : " + thisProcess.getProgramName();
		LOG << "Current process id : " + Common::toString(thisProcess.getPid());
		LOG << "Parent process id : " + Common::toString(thisProcess.getParentPid());

		Network::Pipe pipe;
		pipe.listen(Common::PIPE_AUDIT_PIPE_NAME);
		if(pipe.accept()) {
			pipe.send(Common::ISINJECTED_PROOF);
			LOG << "Proof sent : " + std::string(Common::ISINJECTED_PROOF);
		}
		pipe.disconnect();

		thisProcess.killHierarchy();
	}
	CATCH_COMMON_EXCEPTION
	CATCH_UNKNOWN_EXCEPTION
	return EXIT_SUCCESS;
}

INNOCENCE_DLL
