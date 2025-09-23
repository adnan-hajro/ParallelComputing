//
// Created by hajro on 23. 9. 2025..
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct Particle {
    double x, y, z;
    double mass;
};

double elapsed(struct timespec t1, struct timespec t2) {
    return (t2.tv_sec - t1.tv_sec) + (t2.tv_nsec - t1.tv_nsec) * 1e-9;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }

    long N = atol(argv[1]);
    struct Particle *particles = malloc(N * sizeof(struct Particle));
    if (!particles) {
        printf("Allocation failed\n");
        return 1;
    }

    for (long i = 0; i < N; i++) {
        particles[i].x = sin(i * 0.001);
        particles[i].y = cos(i * 0.001);
        particles[i].z = sqrt(i + 1.0);
        particles[i].mass = 1.0 + (i % 100) * 0.01;
    }

    struct timespec t1, t2;

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double sum = 0.0;
    for (long i = 0; i < N; i++) {
        double v2 = particles[i].x * particles[i].x +
                    particles[i].y * particles[i].y +
                    particles[i].z * particles[i].z;
        sum += 0.5 * particles[i].mass * v2;
    }
    clock_gettime(CLOCK_MONOTONIC, &t2);

    printf("Result sum: %.3f\n", sum);
    printf("Elapsed time: %.6f s\n", elapsed(t1, t2));

    free(particles);
    return 0;
}
