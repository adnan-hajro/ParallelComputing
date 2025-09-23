//
// Created by hajro on 22. 9. 2025..
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void flush_cache(size_t size) {
    char *flush = (char *)malloc(size);
    for (size_t i = 0; i < size; i++) {
        flush[i] = (char)i;
    }
    free(flush);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    int jmax = atoi(argv[1]);
    int imax = atoi(argv[2]);

    printf("Creating contiguous %dx%d matrix\n", jmax, imax);

    double **x = (double **)malloc(jmax * sizeof(double*));
    if (x == NULL) {
        printf("Failed to allocate row pointers\n");
        return 1;
    }

    x[0] = (double *)malloc(jmax * imax * sizeof(double));
    if (x[0] == NULL) {
        printf("Failed to allocate matrix data\n");
        free(x);
        return 1;
    }

    for(int j = 1; j < jmax; j++) {
        x[j] = x[j-1] + imax;
    }

    struct timespec tstart, tstop, tresult;

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    for (int j = 0; j < jmax; j++) {
        for (int i = 0; i < imax; i++) {
            x[j][i] = sin(j * 0.001) + cos(i * 0.001) + sqrt(j + i + 1);
        }
    }

    flush_cache(100 * 1024 * 1024);

    srand(42);
    double sum = 0.0;
    int total = jmax * imax;
    for (int k = 0; k < total; k++) {
        int j = rand() % jmax;
        int i = rand() % imax;
        sum += x[j][i] * 0.5 + sqrt(fabs(x[j][i]));
    }

    clock_gettime(CLOCK_MONOTONIC, &tstop);

    tresult.tv_sec = tstop.tv_sec - tstart.tv_sec;
    tresult.tv_nsec = tstop.tv_nsec - tstart.tv_nsec;

    if (tresult.tv_nsec < 0) {
        tresult.tv_sec--;
        tresult.tv_nsec += 1000000000;
    }

    printf("Matrix sum: %.2f\n", sum);
    printf("Elapsed time: %f seconds\n",
           (double)tresult.tv_sec + (double)tresult.tv_nsec * 1.0e-9);

    free(x[0]);
    free(x);
    return 0;
}