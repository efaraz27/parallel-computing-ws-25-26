#include <stdio.h>
#include <omp.h>

static long global_num_steps = 2500000000;

void sequential_pi(long num_steps) {
    double step = 1.0 / (double)num_steps;
    double x, sum = 0.0;
    double pi;

    double start_time = omp_get_wtime();

    for (long i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    pi = step * sum;

    double end_time = omp_get_wtime();

    printf("Sequential Variant:\n");
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}

void block_decomposition_pi(long num_steps) {
    double step = 1.0 / (double)num_steps;
    double pi, sum = 0.0;

    int max_threads = omp_get_max_threads();
    double partial_sums[max_threads];
    for (long i = 0; i < max_threads; i++) {
        partial_sums[i] = 0.0;
    }
    double start_time = omp_get_wtime();
    
    #pragma omp parallel
    {
        double x;
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        double local_sum = 0.0; 

        long chunk_size = num_steps / num_threads;
        long start = id * chunk_size;

        long end = (id == num_threads - 1) ? num_steps : start + chunk_size;

        for (long i = start; i < end; i++) {
            x = (i + 0.5) * step;
            local_sum = local_sum + 4.0 / (1.0 + x * x);
        }
        
        partial_sums[id] = local_sum;
    }

    for (long i = 0; i < max_threads; i++) {
        sum += partial_sums[i];
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    printf("Block Decomposition Variant:\n");
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}

void cyclic_distribution_pi(long num_steps) {
    double step = 1.0 / (double)num_steps;
    double pi, sum = 0.0;
    int max_threads = omp_get_max_threads();
    double partial_sums[max_threads];
    for (long i = 0; i < max_threads; i++) {
        partial_sums[i] = 0.0;
    }
    double start_time = omp_get_wtime();
    
    #pragma omp parallel
    {
        double x;
        int id = omp_get_thread_num();
        int num_threads = omp_get_num_threads();
        double local_sum = 0.0;

        for (long i = id; i < num_steps; i += num_threads) {
            x = (i + 0.5) * step;
            local_sum = local_sum + 4.0 / (1.0 + x * x);
        }
        
        partial_sums[id] = local_sum;
    }

    for (long i = 0; i < max_threads; i++) {
        sum += partial_sums[i];
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    printf("Cyclic Distribution Variant:\n");
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}


int main() {
    printf("Calculating Pi approximation using N = %ld steps.\n\n", global_num_steps);
    
    // sequential_pi(global_num_steps);
    // printf("\n");
    
    // block_decomposition_pi(global_num_steps);
    // printf("\n");
    
    cyclic_distribution_pi(global_num_steps);

    return 0;
}