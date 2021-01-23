cmake_minimum_required(VERSION 3.16)
project(jasm)

include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

file(GLOB_RECURSE sources "src/*.cpp")

add_library(jasm SHARED ${sources})

target_include_directories(jasm PUBLIC include)
if (IS_BIG_ENDIAN)
    target_compile_definitions(jasm PUBLIC IS_BIG_ENDIAN=1)
else ()
    target_compile_definitions(jasm PUBLIC IS_LITTLE_ENDIAN=1)
endif ()

add_executable(endianess_test test/endianness_test.cpp)
target_link_libraries(endianess_test PUBLIC jasm)