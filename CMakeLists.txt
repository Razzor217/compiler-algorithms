cmake_minimum_required(VERSION 3.10)

project(algorithm)

set(CMAKE_CXX_FLAGS "-std=c++20 -msse4.2 -Wall -Wextra -O3 -g")

add_library(algo_lib INTERFACE)
target_include_directories(algo_lib INTERFACE include/)

add_subdirectory(test)