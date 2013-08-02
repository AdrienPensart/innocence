# Windows audit unit tests
if(WIN32)
	file(GLOB_RECURSE isinjected_source_files isinjected/*)
	add_library(isinjected SHARED ${isinjected_source_files})
	target_link_libraries(isinjected system network common)
	set_target_properties(isinjected PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE injection_source_files injection/*)
	add_executable(injection ${injection_source_files})
	target_link_libraries(injection common malicious system network)
	add_dependencies(injection dll)
	set_target_properties(injection PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE registre_source_files registre/*)
	add_executable(registre ${registre_source_files})
	target_link_libraries(registre system network common)
	set_target_properties(registre PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE rshell_source_files rshell/*)
	add_executable(rshell ${rshell_source_files})
	target_link_libraries(rshell common network)
	set_target_properties(rshell PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE passwords_source_files passwords/*)
	add_executable(passwords ${passwords_source_files})
	target_link_libraries(passwords malicious userenv common network)
	set_target_properties(passwords PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE keyhooker_source_files keyhooker/*)
	add_executable(keyhooker ${keyhooker_source_files})
	target_link_libraries(keyhooker system malicious common network)
	set_target_properties(keyhooker PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE pipe_source_files pipe/*)
	add_executable(pipe ${pipe_source_files})
	target_link_libraries(pipe system common network)
	set_target_properties(pipe PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE hideproc_source_files hideproc/*)
	add_executable(hideproc ${hideproc_source_files})
	target_link_libraries(hideproc system malicious common network)
	set_target_properties(hideproc PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE hidewin_source_files hidewin/*)
	add_executable(hidewin ${hidewin_source_files})
	target_link_libraries(hidewin system malicious common network)
	set_target_properties(hidewin PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE screenshot_source_files screenshot/*)
	add_executable(screenshot ${screenshot_source_files})
	target_link_libraries(screenshot system malicious common network)
	set_target_properties(screenshot PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE elevator_source_files elevator/*)
	add_executable(elevator ${elevator_source_files})
	target_link_libraries(elevator system common network malicious)
	set_target_properties(elevator PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE isadmin_source_files isadmin/*)
	add_executable(isadmin ${isadmin_source_files})
	target_link_libraries(isadmin system common network)
	set_target_properties(isadmin PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
	
	file(GLOB_RECURSE loadslave_source_files loadslave/*)
	add_executable(loadslave ${loadslave_source_files})
	# target_link_libraries(loadslave system common network)
	set_target_properties(loadslave PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
endif ()

# Portable tests
file(GLOB_RECURSE session_source_files session/*)
add_executable(session ${session_source_files})
target_link_libraries(session system blaspheme common network)
set_target_properties(session PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})

file(GLOB_RECURSE mail_source_files mail/*)
add_executable(mail ${mail_source_files})
target_link_libraries(mail system malicious common network)
set_target_properties(mail PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})

file(GLOB_RECURSE transfer_source_files transfer/*)
add_executable(transfer ${transfer_source_files})
target_link_libraries(transfer system network common blaspheme)
set_target_properties(transfer PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
