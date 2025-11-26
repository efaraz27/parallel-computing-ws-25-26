#include <stdio.h>
#include <omp.h>
#include <string.h>

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

void parallel_for_static_pi(long num_steps, int chunk_size) {
    double step = 1.0 / (double)num_steps;
    double pi, sum = 0.0;

    double start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(static, chunk_size)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        double temp = 4.0 / (1.0 + x * x);
        #pragma omp atomic
        sum += temp;
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    if (chunk_size > 0) {
        printf("Static Scheduling (chunk=%d):\n", chunk_size);
    } else {
        printf("Static Scheduling (default chunk):\n");
    }
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}

void parallel_for_dynamic_pi(long num_steps, int chunk_size) {
    double step = 1.0 / (double)num_steps;
    double pi, sum = 0.0;

    double start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(dynamic, chunk_size)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        double temp = 4.0 / (1.0 + x * x);
        #pragma omp atomic
        sum += temp;
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    if (chunk_size > 0) {
        printf("Dynamic Scheduling (chunk=%d):\n", chunk_size);
    } else {
        printf("Dynamic Scheduling (default chunk):\n");
    }
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}

void parallel_for_guided_pi(long num_steps, int chunk_size) {
    double step = 1.0 / (double)num_steps;
    double pi, sum = 0.0;

    double start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(guided, chunk_size)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        double temp = 4.0 / (1.0 + x * x);
        #pragma omp atomic
        sum += temp;
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    if (chunk_size > 0) {
        printf("Guided Scheduling (chunk=%d):\n", chunk_size);
    } else {
        printf("Guided Scheduling (default chunk):\n");
    }
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}

void parallel_for_runtime_pi(long num_steps) {
    double step = 1.0 / (double)num_steps;
    double pi, sum = 0.0;

    double start_time = omp_get_wtime();
    
    #pragma omp parallel for schedule(runtime)
    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        double temp = 4.0 / (1.0 + x * x);
        #pragma omp atomic
        sum += temp;
    }

    pi = step * sum;
    double end_time = omp_get_wtime();

    printf("Runtime Scheduling (controlled by OMP_SCHEDULE):\n");
    printf("Approximation of Pi: %.10f\n", pi);
    printf("Time taken: %.6f seconds\n", end_time - start_time);
}



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <variant>\n", argv[0]);
        printf("Variants:\n");
        printf("  seq  - Sequential execution\n");
        printf("  blk  - Block scheduling (static scheduling)\n");
        printf("  cyc  - Cyclic scheduling (static scheduling with chunk size 1)\n");
        printf("  all  - Run all variants (original behavior)\n");
        return 1;
    }

    char *variant = argv[1];
    
    printf("Calculating Pi approximation using N = %ld steps.\n", global_num_steps);
    printf("Number of threads: %d\n\n", omp_get_max_threads());
    
    if (strcmp(variant, "seq") == 0) {
        sequential_pi(global_num_steps);
    }
    else if (strcmp(variant, "blk") == 0) {
        printf("===== BLOCK SCHEDULING (STATIC) =====\n");
        parallel_for_static_pi(global_num_steps, 0);
        printf("\n");
        
        parallel_for_static_pi(global_num_steps, 1000);
        printf("\n");
        
        parallel_for_static_pi(global_num_steps, 10000);
        printf("\n");
        
        parallel_for_static_pi(global_num_steps, 100000);
        printf("\n");
    }
    else if (strcmp(variant, "cyc") == 0) {
        printf("===== CYCLIC SCHEDULING (STATIC, CHUNK=1) =====\n");
        parallel_for_static_pi(global_num_steps, 1);
    }
    else if (strcmp(variant, "all") == 0) {
        sequential_pi(global_num_steps);
        printf("\n");
        
        printf("===== STATIC SCHEDULING =====\n");
        parallel_for_static_pi(global_num_steps, 0);
        printf("\n");
        
        parallel_for_static_pi(global_num_steps, 1000);
        printf("\n");
        
        parallel_for_static_pi(global_num_steps, 10000);
        printf("\n");
        
        parallel_for_static_pi(global_num_steps, 100000);
        printf("\n");
        
        printf("===== DYNAMIC SCHEDULING =====\n");
        parallel_for_dynamic_pi(global_num_steps, 0);
        printf("\n");
        
        parallel_for_dynamic_pi(global_num_steps, 1000);
        printf("\n");
        
        parallel_for_dynamic_pi(global_num_steps, 10000);
        printf("\n");
        
        parallel_for_dynamic_pi(global_num_steps, 100000);
        printf("\n");

        printf("===== GUIDED SCHEDULING =====\n");
        parallel_for_guided_pi(global_num_steps, 0);
        printf("\n");
        
        parallel_for_guided_pi(global_num_steps, 1000);
        printf("\n");
        
        parallel_for_guided_pi(global_num_steps, 10000);
        printf("\n");
        
        parallel_for_guided_pi(global_num_steps, 100000);
        printf("\n");
        
        printf("Set OMP_SCHEDULE environment variable to control scheduling\n");
        printf("Examples: export OMP_SCHEDULE=\"static,1000\" or \"dynamic,10000\" or \"guided\"\n");
        parallel_for_runtime_pi(global_num_steps);
        printf("\n");
    }
    else {
        printf("Unknown variant: %s\n", variant);
        printf("Valid variants: seq, blk, cyc, all\n");
        return 1;
    }

    return 0;
}