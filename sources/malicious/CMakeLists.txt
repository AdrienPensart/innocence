file(GLOB_RECURSE malicious_source_files *.hpp *.cpp)
add_library(malicious STATIC ${malicious_source_files})
target_link_libraries(malicious common system)
