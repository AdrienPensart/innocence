file(GLOB_RECURSE bucket_source_files *.hpp *.cpp)
add_executable(bucket ${bucket_source_files})
target_link_libraries(bucket system network common blaspheme)
