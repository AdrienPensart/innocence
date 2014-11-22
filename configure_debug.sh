#!/bin/sh

cd build
cmake ../src -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cd ..
