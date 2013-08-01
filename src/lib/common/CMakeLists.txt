file(GLOB_RECURSE common_source_files *.hpp *.cpp)

add_library(common STATIC ${common_source_files})
add_library(common_md STATIC ${common_source_files})

set_target_properties(common PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
set_target_properties(common_md PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MD})
