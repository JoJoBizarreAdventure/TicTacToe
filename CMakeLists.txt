cmake_minimum_required(VERSION 3.15)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif ()

find_package(Python COMPONENTS Interpreter REQUIRED) # trick code for windows in github action, without this jinja2 is missing
message("------------Current Python Path: ${Python_EXECUTABLE}------------")

# global libs
add_subdirectory(libs)

# games list below
add_subdirectory(TicTacToe)