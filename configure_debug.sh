#!/bin/sh

cd build
cmake ../sources -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cd ..
