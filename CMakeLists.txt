cmake_minimum_required(VERSION 3.10)

project(algorithm)

add_library(algo_lib INTERFACE)
target_include_directories(algo_lib INTERFACE include/)

add_subdirectory(test)