file(GLOB_RECURSE network_source_files *.hpp *.cpp)

add_library(network STATIC ${network_source_files})
add_library(network_md STATIC ${network_source_files})

set_target_properties(network PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
set_target_properties(network_md PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})

# Windows needs WinSock library...
if (WIN32)
	target_link_libraries(network common ws2_32)
	target_link_libraries(network_md common_md ws2_32)
endif ()
