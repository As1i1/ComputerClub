cmake_minimum_required(VERSION 3.21)
project(computer_club)

set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_TOOLCHAIN_FILE "/home/aas1i1/Programming/vcpkg/scripts/buildsystems/vcpkg.cmake") # TODO

find_package(GTest CONFIG REQUIRED)

#file(GLOB SRC ../src/*.cpp) # TODO remove main file
file(GLOB EVENT ../src/event/*.cpp)
file(GLOB PARSER ../src/parser/*.cpp)
add_executable(test ${EVENT} ${PARSER} parser_tests.cpp)

target_link_libraries(test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)


