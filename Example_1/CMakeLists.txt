cmake_minimum_required(VERSION 3.22.1)

project(MatrixPerformanceLab C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(fragmented_matrix fragmented_matrix.c)
add_executable(contiguous_matrix contiguous_matrix.c)

target_link_libraries(fragmented_matrix rt m)
target_link_libraries(contiguous_matrix rt m)

enable_testing()

add_test(
        NAME fragmented_matrix_test_large
        COMMAND fragmented_matrix 10000 10000
)

set_tests_properties(fragmented_matrix_test_large PROPERTIES
        TIMEOUT 30
)

add_test(
        NAME contiguous_matrix_test_large
        COMMAND contiguous_matrix 10000 10000
)

set_tests_properties(contiguous_matrix_test_large PROPERTIES
        TIMEOUT 30
)