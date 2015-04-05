# Innocence

## Description

* Stealth Remote Administration Tool
* Audit tool for desktops

## Dependencies

* Qt
* Windows API
* SDK Windows
* Jpeg library
* OTL SQL

## Tools

* Visual Studio 2012/2014
* CMake
* GraphViz
* Doxygen
* Vmware vSphere
* bin2array : create a C array from a file
* buildnum : increment build version
* bnhide.sys
* netcat
* UPX

## Workflow

1. Detect Windows version
2. Privilege escalation
3. Installation
4. Copy exe
5. Start slave module
6. Delete original exe
7. Stealth Internet Explorer instance
8. IE COM instance
9. Start of stealth service
10. DLL Injection into IE
11. Registry startup installation
12. Reverse connection to Master
13. Waiting for connections and executing commands
14. Disconnection
15. Uninstallation
16. Delete registry startup
17. Kill of IE and DLL
18. Auto-delete exe

## Librairies

* imd (not ready) : innocence malicious driver, stealth services, packets injection and capture
* common : design patterns, en gros ce qui est commun a toutes les libraires et executables
* logger : console and remote logging
* network : mini network wrapper Unix/Windows
* blaspheme : innocence protocol : session, authentification, encryption, file transfer
* system : windows wrapper for registry, threads, files
* keyboard : keylogging library
* elevation32.exe et elevation64.exe : privilege escalation exploit
* elevation32.dll et elevation64.dll : privilege escalation DLL
* malicious : DLL injector, stealth IE, password stealing, stealth processes, binary integration

## Modules

* inhibiter : innocence installer
* slave_dll : innocence DLL
* master : GUI, can controll multiple innocence slaves
* logger_gui : view logs in GUI
* logger : view logs in console

## Audit tests

* isinjected : DLL injection (malicious)
* isadmin : am i admin ?
* elevation : privilege escalation
* rshell : reverse shell
* mail : is mailing possible ?
* transfer : file transfer testing
* passwords : dump all passwords
* pipe : windows IPC test
* session : blaspheme protocol test
* keyhooker : keylogger test
* hideproc : hide some processus with imd driver
* hidewin : IE stealth test
* injection : is DLL injection working ?
* registre : is registry writable ?
* screenshot : screenshot test

## ToDo

* rewrite imd
* file size for transfers
* replace IP with DNS in reverse connection
* Test cluster in PCC and rapport creation
* Unit testing
* Rewrite the password stealer library
* Patch startup executable instead of using registry keys

