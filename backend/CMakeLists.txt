cmake_minimum_required(VERSION 3.60)
project(webtop CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(webtop-back src/cpu.cpp src/webtop.cpp)

target_include_directories(webtop-back PRIVATE include)