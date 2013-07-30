file(GLOB_RECURSE blaspheme_source_files *.hpp *.cpp)
add_library(blaspheme STATIC ${blaspheme_source_files})
target_link_libraries(blaspheme system network common)
