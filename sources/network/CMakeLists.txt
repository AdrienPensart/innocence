cmake_minimum_required(VERSION 2.8)
project(network)

file(GLOB_RECURSE network_source_files *.hpp *.cpp)
add_library(network	STATIC ${network_source_files})

if (WIN32)
	target_link_libraries(network ws2_32)
endif ()
