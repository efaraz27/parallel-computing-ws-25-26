#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 700
#define THREADS 2

int main() {
    int i, j, k;
    double sum = 0.0;
    double start = 0.0, end = 0.0;
    double *a = malloc(N * N * sizeof(double));
    double *b = malloc(N * N * sizeof(double));
    double *c = malloc(N * N * sizeof(double));

    if (!a || !b || !c) {
        fprintf(stderr, "Memory allocation failed\n");
        free(a);
        free(b);
        free(c);
        return 1;
    }

    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            a[i * N + j] = 3.0 * i + j;
            b[i * N + j] = 5.2 * i + 2.3 * j;
            c[i * N + j] = 0.0;
        }
    }
    omp_set_num_threads(THREADS);
    start = omp_get_wtime();
    #pragma omp parallel for shared(a, b, c) private(i, j, k) schedule(static)
    for(i = 0; i < N; i++){
        for(j = 0; j < N; j++){
            for(k = 0; k < N; k++){
                c[i * N + j] += a[i * N + k] * b[k * N + j];
            }
        }
    }
    end = omp_get_wtime();

    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            sum += c[i * N + j];
        }
    }
    printf("Result: %f\n", sum);
    printf("Elapsed time: %f seconds\n", end - start);
    free(a);
    free(b);
    free(c);
    return 0;
}