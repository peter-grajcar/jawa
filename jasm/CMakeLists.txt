# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2021 Peter Grajcar
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

add_subdirectory(test)
