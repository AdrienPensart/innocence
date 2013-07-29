file(GLOB_RECURSE auditor_source_files *.cpp *.hpp)
add_executable(auditor ${auditor_source_files})
target_link_libraries(auditor system network common)