cmake_minimum_required(VERSION 3.22)

project(lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(COMPILER_FLAGS "/W4" "/utf-8")
else()
    set(COMPILER_FLAGS "-Wall" "-Wextra" "-Wconversion" "-Wpedantic")
endif()

add_executable(lab1 main.cpp)

target_compile_options(lab1 PRIVATE ${COMPILER_FLAGS})