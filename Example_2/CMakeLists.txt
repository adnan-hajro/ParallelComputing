cmake_minimum_required(VERSION 3.22.1)

project(ArrayOfStructures C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(array_of_structures array_of_structures.c)

target_link_libraries(array_of_structures rt m)

enable_testing()

add_test(
        NAME array_of_structures_test
        COMMAND array_of_structures 10000000
)

set_tests_properties(array_of_structures_test PROPERTIES
        TIMEOUT 10
)