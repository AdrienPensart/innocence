#!/bin/sh

mkdir build
cd build
cmake ../sources -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cd ..
