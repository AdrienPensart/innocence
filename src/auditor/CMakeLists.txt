file(GLOB_RECURSE auditor_agent_source_files agent/* Auditor.hpp)
add_executable(auditor_agent ${auditor_agent_source_files})
target_link_libraries(auditor_agent system network common)
set_target_properties(auditor_agent PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})

file(GLOB_RECURSE auditor_server_source_files server/* Auditor.hpp)
add_executable(auditor_server ${auditor_server_source_files})
target_link_libraries(auditor_server system network common)
set_target_properties(auditor_server PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS_MT})
