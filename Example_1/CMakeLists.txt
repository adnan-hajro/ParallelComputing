cmake_minimum_required(VERSION 3.22.1)
project(TimeIt)

set(CMAKE_C_STANDARD 17)

find_package(MPI REQUIRED)

add_executable(TimeIt TimeIt.c)

add_executable(MPITimeIt MPITimeIt.c)

# target_include_directories(MPITimeIt PUBLIC MPI::MPI_C)
target_link_libraries(MPITimeIt PRIVATE MPI::MPI_C)

enable_testing()

add_test(NAME serial_timing_test COMMAND TimeIt)
add_test(NAME mpi_single_process COMMAND mpirun -n 1 MPITimeIt)
add_test(NAME mpi_dual_process COMMAND mpirun -n 2 MPITimeIt)

add_custom_target(run_all_timing
        COMMAND echo "Running Serial Version:"
        COMMAND ./TimeIt
        COMMAND echo "Running MPI with 1 process:"
        COMMAND mpirun -n 1 ./MPITimeIt
        COMMAND echo "Running MPI with 2 processes:"
        COMMAND mpirun -n 2 ./MPITimeIt
        DEPENDS TimeIt MPITimeIt
)

add_custom_target(run_mpi_timing
        COMMAND mpirun -n 2 $<TARGET_FILE:MPITimeIt>
        DEPENDS MPITimeIt
        COMMENT "Running MPI timing test with 2 processes"
)

