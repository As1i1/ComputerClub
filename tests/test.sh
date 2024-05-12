#!/bin/bash
cmake  -B=build-cmake -DCMAKE_TOOLCHAIN_FILE="/home/aas1i1/Programming/vcpkg/scripts/buildsystems/vcpkg.cmake" # TODO
cmake --build build-cmake
./build-cmake/test