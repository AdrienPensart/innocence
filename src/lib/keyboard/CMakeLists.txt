file(GLOB_RECURSE keyboard_source_files *.hpp *.cpp)
add_library(keyboard STATIC ${keyboard_source_files})
if(WIN32)
    set_target_properties(keyboard PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
endif()

target_link_libraries(keyboard log network system)
