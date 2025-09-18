#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
    struct timespec tstart, tstop;
    double elapsed_time;

    printf("=== Serial Timing Test ===\n");

    // Record start time
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    // Do some work (sleep for 3 seconds)
    printf("Working for 3 seconds...\n");
    sleep(3);

    // Record end time
    clock_gettime(CLOCK_MONOTONIC, &tstop);

    // Calculate elapsed time
    elapsed_time = (tstop.tv_sec - tstart.tv_sec) +
                   (tstop.tv_nsec - tstart.tv_nsec) / 1.0e9;

    printf("Elapsed time: %.9f seconds\n", elapsed_time);
    printf("Expected: ~3.000000000 seconds\n");

    return 0;
}