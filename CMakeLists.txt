cmake_minimum_required(VERSION 4.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(CraniumFuck)

file(GLOB_RECURSE SRC "src/*.c")

add_executable(cf ${SRC})

#useless but doesnt hurt anyone ;)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_NAME "GCC")
else()
    set(COMPILER_NAME "${CMAKE_C_COMPILER_ID}")
endif()

target_compile_definitions(cf PRIVATE
    CMAKE_COMPILER_NAME="${COMPILER_NAME}"
    CMAKE_COMPILER_VERSION="${CMAKE_C_COMPILER_VERSION}"
)

target_include_directories(cf PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_compile_options(cf PRIVATE -Wall -Wextra)
