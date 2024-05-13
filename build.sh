#!/bin/bash
cd src
cmake . -B=build-cmake-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake-release