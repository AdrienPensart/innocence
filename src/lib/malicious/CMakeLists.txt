set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
link_directories(${CMAKE_SOURCE_DIR}/lib/jpeg)

# we have an external JPEG library
add_library(myjpeg UNKNOWN IMPORTED) 
set_target_properties(myjpeg PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/jpeg/jpeg.lib")

file(GLOB_RECURSE malicious_source_files *.hpp *.cpp)
add_library(malicious STATIC ${malicious_source_files})
set_target_properties(malicious PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})

# generation of privilege escalation executable 32bit
add_custom_command(
	TARGET malicious
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATOR32_EXE_PATH}" "${ELEVATOR32_HPP_PATH}" "Elevator32"
	COMMENT "Generating ${ELEVATOR_HPP32_PATH} WITH ${ELEVATOR32_EXE_PATH}"
)

# generation of privilege escalation DLL 32bit
add_custom_command(
	TARGET malicious
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATORDLL32_DLL_PATH}" "${ELEVATORDLL32_HPP_PATH}" "ElevatorDll32"
	COMMENT "Generating ${ELEVATORDLL32_HPP_PATH} WITH ${ELEVATORDLL32_DLL_PATH}"
)

# generation of privilege escalation executable 64bit
add_custom_command(
	TARGET malicious
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATOR64_EXE_PATH}" "${ELEVATOR64_HPP_PATH}" "Elevator64"
	COMMENT "Generating ${ELEVATOR64_HPP_PATH} WITH ${ELEVATOR64_EXE_PATH}"
)

# generation of privilege escalation DLL 64bit
add_custom_command(
	TARGET malicious
	PRE_BUILD
	COMMAND "${BINGENERATOR}" "${ELEVATORDLL64_DLL_PATH}" "${ELEVATORDLL64_HPP_PATH}" "ElevatorDll64"
	COMMENT "Generating ${ELEVATORDLL64_HPP_PATH} WITH ${ELEVATORDLL64_DLL_PATH}"
)

target_link_libraries(malicious common system myjpeg)
