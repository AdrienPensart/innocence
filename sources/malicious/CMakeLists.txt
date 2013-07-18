set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
link_directories(${CMAKE_SOURCE_DIR}/utils/jpeg)

add_library(myjpeg UNKNOWN IMPORTED) 
set_target_properties(myjpeg PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/utils/jpeg/jpeg.lib")

file(GLOB_RECURSE malicious_source_files *.hpp *.cpp)
add_library(malicious STATIC ${malicious_source_files})
target_link_libraries(malicious common system myjpeg)
