file(GLOB_RECURSE keyboard_source_files *.hpp *.cpp)
add_library(keyboard STATIC ${keyboard_source_files})
set_target_properties(keyboard PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})

target_link_libraries(keyboard log network system)
