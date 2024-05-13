cmake_minimum_required(VERSION 3.21)
project(computer_club)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC *.cpp)
file(GLOB PARSER parser/*.cpp)
file(GLOB PROCESSING event_processing.cpp)
add_executable(${PROJECT_NAME} ${SRC} ${EVENT} ${PARSER} ${PROCESSING})