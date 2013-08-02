cmake_minimum_required(VERSION 2.8.11)
project(buildnum)

file(GLOB_RECURSE buildnum_source_files src/*)
add_executable(buildnum ${buildnum_source_files})
