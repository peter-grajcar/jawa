# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Copyright (c) 2021 Peter Grajcar
cmake_minimum_required(VERSION 3.16)
project(jawa)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("jawa")
add_subdirectory("jasm")
add_subdirectory("stdbib")

# Doxygen setup
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_CONFIG docs/doxygen-config)
    add_custom_target(docs ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
            WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else ()
    message("Doxygen not found.")
endif ()