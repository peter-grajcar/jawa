# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2021 Peter Grajcar
add_executable(endianess_test endianness_test.cpp)
target_link_libraries(endianess_test PUBLIC jasm)

add_executable(class_reader_test class_reader_test.cpp)
target_link_libraries(class_reader_test PUBLIC jasm)

add_executable(type_test type_test.cpp)
target_link_libraries(type_test PUBLIC jasm)

add_executable(class_builder_test class_builder_test.cpp)
target_link_libraries(class_builder_test PUBLIC jasm)