cmake_minimum_required(VERSION 3.7)
project(Capstone)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Capstone ${SOURCES})
