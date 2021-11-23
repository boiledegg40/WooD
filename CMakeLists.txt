cmake_minimum_required(VERSION 3.10)

project(doom_game)

set(CMAKE_CXX_FLAGS "-Wall")

set(CMAKE_CXX_STANDARD 14)

find_package(glfw3 3.3 REQUIRED)

add_executable(WooD)

target_include_directories(WooD PRIVATE include)

add_subdirectory(src)

target_link_libraries(WooD glfw)

