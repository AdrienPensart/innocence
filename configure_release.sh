#!/bin/sh

mkdir -p build
cd build
cmake ../src -G "Unix Makefiles" -Wno-dev -DCMAKE_BUILD_TYPE=Release
cd ..
