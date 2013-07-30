file(GLOB_RECURSE auditor_agent_source_files agent/*)
add_executable(auditor_agent ${auditor_agent_source_files})
target_link_libraries(auditor_agent system network common)

file(GLOB_RECURSE auditor_server_source_files server/*)
add_executable(auditor_server ${auditor_server_source_files})
target_link_libraries(auditor_server system network common)
