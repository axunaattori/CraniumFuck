cmake_minimum_required(VERSION 4.0)
project(CraniumFuck)

file(GLOB SRC "src/*.c")

add_executable(cf ${SRC})

target_compile_options(cf PRIVATE -Wall -Wextra)
