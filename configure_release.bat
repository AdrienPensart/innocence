mkdir build
cd build
cmake ../src -Wno-dev -G"Visual Studio 11" -T"v110_xp" -DCMAKE_BUILD_TYPE=Release
cd ..
