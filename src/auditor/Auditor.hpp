#ifndef _AUDITOR_
#define _AUDITOR_

#include <network/Types.hpp>

static const char * AUDIT_COLLECTOR_IP = "127.0.0.1";
Network::Port AUDIT_COLLECTOR_PORT = 80;

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

#endif // _AUDITOR_
