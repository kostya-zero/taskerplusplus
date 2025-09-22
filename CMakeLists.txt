cmake_minimum_required(VERSION 4.0)
project(taskerplusplus)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(taskerplusplus
        src/main.cpp
)
