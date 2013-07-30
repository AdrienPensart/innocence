cmake_minimum_required(VERSION 2.6)
project(bin2array)

file(GLOB_RECURSE bin2array_source_files src/*.cpp src/*.hpp)
add_executable(bin2array ${bin2array_source_files})
