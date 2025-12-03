#include <stdio.h>
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

double parallel_pi_reduction(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}


double parallel_pi_critical_naive(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double x, tmp;

        #pragma omp for
        for (long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            tmp = 4.0 / (1.0 + x * x);

            #pragma omp critical
            {
                sum += tmp;
            }
        }
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

double parallel_pi_critical_chunked(long num_steps, double *time_out) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        double local_sum = 0.0;

        #pragma omp for
        for (long i = 0; i < num_steps; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp critical
        {
            sum += local_sum;
        }
    }

    double pi = step * sum;
    double end_time = omp_get_wtime();
    *time_out = end_time - start_time;
    return pi;
}

int main(int argc, char *argv[]) {

    printf("Pi approximation with N = %ld steps\n", global_num_steps);
    printf("Using OpenMP, comparing different parallelization strategies.\n\n");

    double t_seq, pi_seq;
    omp_set_num_threads(1);
    pi_seq = sequential_pi(global_num_steps, &t_seq);

    printf("Sequential baseline:\n");
    printf("  Pi = %.12f, time = %.6f s (1 thread)\n\n", pi_seq, t_seq);

    int thread_counts[] = {1, 2, 4, 8};
    int num_thread_variants = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Performance comparison table:\n");
    printf("| %-25s | %7s | %12s | %12s |\n",
           "Variant", "Threads", "Time [s]", "Pi");

    for (int i = 0; i < num_thread_variants; i++) {
        int threads = thread_counts[i];
        double pi, t;

        omp_set_num_threads(threads);
        pi = parallel_pi_reduction(global_num_steps, &t);
        printf("| %-25s | %7d | %12.6f | %12.10f |\n",
               "parallel_reduction", threads, t, pi);

        // omp_set_num_threads(threads);
        // pi = parallel_pi_critical_naive(global_num_steps, &t);
        // printf("| %-25s | %7d | %12.6f | %12.10f |\n",
        //        "parallel_critical_naive", threads, t, pi);

        // omp_set_num_threads(threads);
        // pi = parallel_pi_critical_chunked(global_num_steps, &t);
        // printf("| %-25s | %7d | %12.6f | %12.10f |\n",
        //        "parallel_critical_chunked", threads, t, pi);
    }

    return 0;
}


