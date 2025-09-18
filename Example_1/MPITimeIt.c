#include <unistd.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    double start_time, end_time, elapsed_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("=== MPI Timing Test ===\n");
        printf("Number of processes: %d\n", size);
    }

    // Synchronize all processes before timing
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Do some work (sleep for 3 seconds)
    if (rank == 0) {
        printf("Working for 3 seconds...\n");
    }
    sleep(3);

    // Synchronize again and stop timing
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();
    elapsed_time = end_time - start_time;

    // Only rank 0 prints the result
    if (rank == 0) {
        printf("Elapsed time: %.9f seconds\n", elapsed_time);
        printf("Expected: ~3.000000000 seconds\n");
    }

    MPI_Finalize();
    return 0;
}