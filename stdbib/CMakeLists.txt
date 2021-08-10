find_package(jni)

file(GLOB_RECURSE native_sources "./*.cpp")
file(GLOB_RECURSE jawa_sources "./*.jawa")

add_custom_target(jawa_stdbib_classes ALL)
add_dependencies(jawa_stdbib_classes jawac)
foreach (filename ${jawa_sources})
    add_custom_command(
            TARGET jawa_stdbib_classes
            COMMAND ${CMAKE_BINARY_DIR}/jawa/jawac --ścieżkaklasy .:${JAVA_CLASSPATH} ${filename}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/stdbib
            VERBATIM
    )
endforeach ()

add_library(jawa_stdbib_native SHARED ${native_sources})
include_directories(jawa_stdbib_native PUBLIC .)
include_directories(jawa_stdbib_native PUBLIC ${JAVA_INCLUDE_PATH})
target_link_libraries(jawa_stdbib_native PUBLIC ${JAVA_JVM_LIBRARY})

add_custom_target(jawa_stdbib ALL)
add_dependencies(jawa_stdbib jawa_stdbib_native jawa_stdbib_classes)
