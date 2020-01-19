cmake_minimum_required (VERSION 3.0)

project(newsh)

add_compile_options(-std=c++14 -g -Wall -Wextra)


include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp src/command.cpp src/newsh.cpp)

