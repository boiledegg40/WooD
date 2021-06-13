cmake_minimum_required(VERSION 3.10)

project(doom_game)

set(CMAKE_CXX_FLAGS "-Wall -lGL -lGLU -lglut")

set(CMAKE_CXX_STANDARD 14)

add_executable(WooD)

add_subdirectory(src)

target_include_directories(WooD PRIVATE include)


