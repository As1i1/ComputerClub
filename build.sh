#!/bin/bash
cd src
cmake . -B=build-cmake-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-cmake-release
mv build-cmake-release/computer_club ../