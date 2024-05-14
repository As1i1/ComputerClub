cmake_minimum_required(VERSION 3.21)
project(computer_club)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest CONFIG REQUIRED)

file(GLOB EVENT ../src/event/*.cpp)
file(GLOB PARSER ../src/parser/*.cpp)
file(GLOB PROCESS ../src/event_processing.cpp)
file(GLOB TESTS ../tests/*.cpp)
add_executable(test ${EVENT} ${PARSER} ${TESTS} ${PROCESS})

target_link_libraries(test PRIVATE GTest::gtest GTest::gtest_main)

