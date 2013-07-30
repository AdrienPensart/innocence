if(WIN32)
	file(GLOB_RECURSE system_source_files *.hpp *.cpp)
else()
	file(GLOB_RECURSE system_source_files File.hpp File.cpp)
endif()

add_library(system STATIC ${system_source_files})
target_link_libraries(system common)
