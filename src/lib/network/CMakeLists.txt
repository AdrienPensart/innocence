file(GLOB_RECURSE network_source_files *.hpp *.cpp)

add_library(network STATIC ${network_source_files})

if(WIN32)
    add_library(network_md STATIC ${network_source_files})
    set_target_properties(network PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
    set_target_properties(network_md PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})
	target_link_libraries(network ws2_32)
	target_link_libraries(network_md ws2_32)
endif ()
