cmake_minimum_required(VERSION 3.16)
project(jawa)

set(CMAKE_CXX_STANDARD 14)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(lexer src/lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)

add_executable(jawa
        src/main.cpp
        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS}
        )

target_include_directories(jawa PUBLIC include)
target_include_directories(jawa PUBLIC build)
