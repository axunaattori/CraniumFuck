cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(CraniumFuck)

file(GLOB_RECURSE SRC "src/*.c")

add_executable(cf ${SRC})

target_include_directories(cf PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_compile_options(cf PRIVATE -Wall -Wextra)
