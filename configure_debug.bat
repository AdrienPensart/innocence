mkdir build
cd build
cmake ../src -Wno-dev -G"Visual Studio 11" -DCMAKE_BUILD_TYPE=Debug -T"v110_xp"
cd ..
