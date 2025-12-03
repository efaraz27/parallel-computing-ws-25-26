#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

static long global_num_steps = 25000000L;

double sequential_pi(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double x, sum = 0.0;

    double start_time = omp_get_wtime();

    for (long i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_static_default(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_static(long num_steps, int chunk_size, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(static, chunk_size)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_dynamic_default(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_dynamic(long num_steps, int chunk_size, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_guided_default(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(guided)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_guided(long num_steps, int chunk_size, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(guided, chunk_size)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_runtime(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(runtime)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

int main(int argc, char *argv[]) {
    // ./pi_par_loop <variant> <threads> <chunk>

    if (argc == 4) {
        const char *variant = argv[1];
        int threads = atoi(argv[2]);
        int chunk = atoi(argv[3]);

        if (threads <= 0) {
            fprintf(stderr, "Error: threads must be > 0\n");
            return 1;
        }

        double t, pi;

        if (strcmp(variant, "seq") == 0) {
            printf("Running sequential variant\n");
            omp_set_num_threads(1);
            pi = sequential_pi(global_num_steps, &t);
        } else if (strcmp(variant, "static") == 0) {
            printf("Running static variant\n");
            omp_set_num_threads(threads);
            if (chunk <= 0) {
                pi = parallel_pi_static_default(global_num_steps, &t);
            } else {
                pi = parallel_pi_static(global_num_steps, chunk, &t);
            }
        } else if (strcmp(variant, "dynamic") == 0) {
            printf("Running dynamic variant\n");
            omp_set_num_threads(threads);
            if (chunk <= 0) {
                pi = parallel_pi_dynamic_default(global_num_steps, &t);
            } else {
                pi = parallel_pi_dynamic(global_num_steps, chunk, &t);
            }
        } else if (strcmp(variant, "guided") == 0) {
            printf("Running guided variant\n");
            omp_set_num_threads(threads);
            if (chunk <= 0) {
                pi = parallel_pi_guided_default(global_num_steps, &t);
            } else {
                pi = parallel_pi_guided(global_num_steps, chunk, &t);
            }
        } else if (strcmp(variant, "runtime") == 0) {
            printf("Running runtime variant\n");
            omp_set_num_threads(threads);
            pi = parallel_pi_runtime(global_num_steps, &t);
        } else {
            fprintf(stderr,
                    "Unknown variant '%s'. Use one of: seq, static, dynamic, guided, runtime\n",
                    variant);
            return 1;
        }

        printf("Single experiment:\n");
        printf("  N        = %ld\n", global_num_steps);
        printf("  variant  = %s\n", variant);
        printf("  threads  = %d\n", (strcmp(variant, "seq") == 0) ? 1 : threads);
        if (strcmp(variant, "seq") != 0 && strcmp(variant, "runtime") != 0) {
            printf("  chunk    = %d (%s)\n",
                   chunk,
                   (chunk <= 0) ? "default" : "explicit");
        }
        if (strcmp(variant, "runtime") == 0) {
            printf("  schedule = runtime (controlled via OMP_SCHEDULE)\n");
        }
        printf("  Pi       = %.12f\n", pi);
        printf("  time     = %.6f s\n", t);

        return 0;
    }

    return 0;
}

