file(GLOB_RECURSE logger_source_files src/*)
add_executable(logger ${logger_source_files})
target_link_libraries(logger system network common)
set_target_properties(logger PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
