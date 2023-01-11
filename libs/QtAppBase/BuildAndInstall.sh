#
# Pass -DCMAKE_PREFIX_PATH=../Toolchains/Qt5.15/5.15.2/gcc_64/lib/cmake 
# in order to use a custom Qt version. Otherwise, system's version will be used.
# Also choose build type

rm -R build/Release
mkdir build/Release
cd build/Release
cmake ../../ -DCMAKE_BUILD_TYPE=Release "$@"
make -j `nproc --ignore 1`
