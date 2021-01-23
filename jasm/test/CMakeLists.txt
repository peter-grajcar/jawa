add_executable(endianess_test endianness_test.cpp)
target_link_libraries(endianess_test PUBLIC jasm)

add_executable(class_reader_test class_reader_test.cpp)
target_link_libraries(class_reader_test PUBLIC jasm)

add_executable(type_test type_test.cpp)
target_link_libraries(type_test PUBLIC jasm)