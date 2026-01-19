cmake_minimum_required(VERSION 3.20)

project(naive_interpolator)

find_package(TBB REQUIRED)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(ni INTERFACE)
add_library(ni::ni ALIAS ni)

target_include_directories(
        ni INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(test)