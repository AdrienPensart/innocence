#!/bin/sh

mkdir -p build
cd build
cmake ../src -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cd ..
