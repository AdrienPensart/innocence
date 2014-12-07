file(GLOB_RECURSE log_source_files *.hpp *.cpp)

add_library(log STATIC ${log_source_files})

if(WIN32)
    add_library(log_md STATIC ${log_source_files})
    target_link_libraries(log system network odbc32.lib)
    target_link_libraries(log_md system_md network_md odbc32.lib)
    set_target_properties(log PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
    set_target_properties(log_md PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})
else()
    target_link_libraries(log network)
endif()
