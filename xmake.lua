set_project("tasker")
set_languages("c++14")
set_toolchains("clang")

target("tasker")
set_kind("binary")
add_files("src/*.cpp")

add_cxflags("-Wall", "-Wextra")
