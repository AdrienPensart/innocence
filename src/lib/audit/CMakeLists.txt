file(GLOB_RECURSE audit_source_files *.hpp *.cpp)

add_library(audit STATIC ${audit_source_files})
if(WIN32)
    add_library(audit_md STATIC ${audit_source_files})
    set_target_properties(audit PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
    set_target_properties(audit_md PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})
    target_link_libraries(audit_md system_md network_md log_md)
endif()

target_link_libraries(audit system network log)
