file(GLOB_RECURSE blaspheme_source_files *.hpp *.cpp)

add_library(blaspheme STATIC ${blaspheme_source_files})
if(WIN32)
    add_library(blaspheme_md STATIC ${blaspheme_source_files})
    set_target_properties(blaspheme PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
    set_target_properties(blaspheme_md PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})
    target_link_libraries(blaspheme_md system_md network_md log_md)
endif()

target_link_libraries(blaspheme system network log)

