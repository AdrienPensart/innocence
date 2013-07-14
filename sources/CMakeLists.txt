cmake_minimum_required(VERSION 2.8.1)
project(innocence CXX)

if ( CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "Mode Debuggage actif.")
	add_definitions(-DINNOCENCE_DEBUG)
elseif( CMAKE_BUILD_TYPE MATCHES Release)
	message(STATUS "Mode Release actif.")	
endif ()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
	message(STATUS "IDE Visual Studio Detected.")
    set_target_properties (${NAME} PROPERTIES PREFIX "../")
endif()

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

message(STATUS "Repertoire d'include supplementaire : ${CMAKE_SOURCE_DIR}")
message(STATUS "Repertoire de sortie Debug : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}")
message(STATUS "Repertoire de sortie Release : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")
message(STATUS "Repertoire de l'arbre de construction : ${PROJECT_BINARY_DIR}")
message(STATUS "Repertoire de l'arbre des sources : ${PROJECT_SOURCE_DIR}")
message(STATUS "Repertoire de sortie runtime : ${RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Repertoire de sortie des modules : ${MODULE_OUTPUT_DIRECTORY}")

add_subdirectory(network) # Construction du wrapper réseau
add_subdirectory(common) # Construction de la librairie de log / debugging
add_subdirectory(system) # Construction du wrapper d'objets système

if(WIN32)
	add_subdirectory(malicious) # Librairie de code malicieuse pour le client
	add_subdirectory(inhibition) # Construction du client (DLL + Injecteur)
endif()

add_subdirectory(blaspheme) # Librairie de defintion du protocole Blaspheme	
add_subdirectory(debugger) # Executable de debuggage par message de log
add_subdirectory(bucket) # Construction du client en mode console
add_subdirectory(thesleeper) # Construction du client en mode QT

# Tests unitaires
if ( CMAKE_BUILD_TYPE MATCHES Debug)
	add_subdirectory(testing)
endif()

# Generation du package
#include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "innocence")
set(CPACK_PACKAGE_VENDOR "CrunchEngine")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/../documentation/ReadMe.txt")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/../documentation/Copyright.txt")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\thesleeper\\\\images\\\\biohazard.jpg")
set(CPACK_PACKAGE_EXECUTABLES "" "thesleeper.exe")

if(WIN32 AND NOT UNIX)
	# Génération du package pour windows
else(WIN32 AND NOT UNIX)
	# Génération du package pour linux
endif(WIN32 AND NOT UNIX)

include(CPack)
