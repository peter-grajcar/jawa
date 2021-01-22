cmake_minimum_required(VERSION 3.16)
project(jawa)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(parser
        src/parser.y
        ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        COMPILE_FLAGS "--report=all --report-file=${CMAKE_CURRENT_BINARY_DIR}/bison.out")
FLEX_TARGET(lexer
        src/lexer.l
        ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)


file(GLOB_RECURSE sources "src/*.cpp")

add_executable(jawac
        ${sources}
        ${BISON_parser_OUTPUTS}
        ${FLEX_lexer_OUTPUTS})

target_include_directories(jawac PUBLIC include)
target_include_directories(jawac PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(jawac PRIVATE jasm)
