cmake_minimum_required(VERSION 2.8.11)
project(bin2array)

file(GLOB_RECURSE bin2array_source_files src/*)
add_executable(bin2array ${bin2array_source_files})
