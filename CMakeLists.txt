cmake_minimum_required(VERSION 3.15)

project(SummerPrj)

set(CMAKE_CXX_STANDARD 17)

add_executable(
    SummerPrj
    src/main.cpp
)

target_include_directories(
    SummerPrj PRIVATE
    Crow/include
    /opt/homebrew/include
)