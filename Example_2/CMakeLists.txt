cmake_minimum_required(VERSION 3.22.1)
project(MemoryLeak)

set(CMAKE_C_STANDARD 17)

add_executable(MemoryLeak MemoryLeak.c)
add_executable(MemoryLeakFix MemoryLeakFix.c)


add_custom_target(run_valgrind
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
        $<TARGET_FILE:MemoryLeak>
        DEPENDS MemoryLeak
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(run_valgrind_fix
        COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
        $<TARGET_FILE:MemoryLeakFix>
        DEPENDS MemoryLeakFix
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
