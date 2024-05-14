#!/bin/bash
cd -- "$( dirname -- "${BASH_SOURCE[0]}" )"
cd tests
cmake  -B=build-cmake-release -DCMAKE_TOOLCHAIN_FILE="$1/vcpkg/scripts/buildsystems/vcpkg.cmake" -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake-release
./build-cmake/test