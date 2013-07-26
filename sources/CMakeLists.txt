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

# For Visual Studio
# the jpeg library needs it to be linked with project
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

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

message(STATUS "Additional include : ${CMAKE_SOURCE_DIR}")
message(STATUS "Debug output in : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "Release output in : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "Build in : ${PROJECT_BINARY_DIR}")
message(STATUS "Sources in : ${PROJECT_SOURCE_DIR}")
message(STATUS "Runtime in : ${RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Modules output in : ${MODULE_OUTPUT_DIRECTORY}")

if(WIN32)
	add_subdirectory(malicious)
	add_subdirectory(inhibition)
endif()

add_subdirectory(network)
add_subdirectory(common)
add_subdirectory(system)
add_subdirectory(auditor)
add_subdirectory(blaspheme)
add_subdirectory(debugger)
add_subdirectory(bucket)
add_subdirectory(thesleeper)

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
