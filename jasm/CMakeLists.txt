cmake_minimum_required(VERSION 3.16)
project(jasm)

file(GLOB_RECURSE sources "src/*.cpp")

add_library(jasm SHARED ${sources})

target_include_directories(jasm PRIVATE include)
