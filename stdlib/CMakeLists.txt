find_package(jni)

file(GLOB_RECURSE sources "./*.cpp")

add_library(jawa_stdlib SHARED ${sources})
include_directories(jawa_stdlib PUBLIC .)
include_directories(jawa_stdlib PUBLIC ${JAVA_INCLUDE_PATH})
target_link_libraries(jawa_stdlib PUBLIC ${JAVA_JVM_LIBRARY})
