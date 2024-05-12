cmake_minimum_required(VERSION 3.21)
project(computer_club)

set(CMAKE_CXX_STANDARD 20)

file(GLOB SRC src/*.cpp)
file(GLOB EVENT src/event/*.cpp)
file(GLOB PARSER src/parser/*.cpp)
add_executable(${PROJECT_NAME} ${SRC} ${EVENT} ${PARSER})

add_subdirectory(tests)