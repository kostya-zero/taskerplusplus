cmake_minimum_required(VERSION 4.0)
project(taskerplusplus)

set(CMAKE_CXX_STANDARD 17)
find_package(nlohmann_json CONFIG REQUIRED)
include_directories(src)

add_executable(taskerplusplus
        src/main.cpp
        src/store.cpp
        src/store.h
)

target_link_libraries(taskerplusplus PRIVATE nlohmann_json::nlohmann_json)
