cmake_minimum_required(VERSION 3.11.3)
project(Capstone)

add_definitions(-std=c++17)
add_definitions(-pthread)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS}")
include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Capstone ${SOURCES})
