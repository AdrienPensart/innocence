file(GLOB_RECURSE common_source_files *.hpp *.cpp)
add_library(common STATIC ${common_source_files})
target_link_libraries(common)
