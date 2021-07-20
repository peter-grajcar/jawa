find_package(jni)

file(GLOB_RECURSE sources "./*.cpp")

add_library(jawa_stdbib SHARED ${sources})
include_directories(jawa_stdbib PUBLIC .)
include_directories(jawa_stdbib PUBLIC ${JAVA_INCLUDE_PATH})
target_link_libraries(jawa_stdbib PUBLIC ${JAVA_JVM_LIBRARY})
