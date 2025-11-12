#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv)
{

    omp_set_num_threads(4);
    #pragma omp parallel
    {
        printf("I am parallel, thread number: %d, total threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
        printf("I am not parallel, thread number: %d, total threads: %d\n", omp_get_thread_num(), omp_get_num_threads());
    }
    // printf("I am not parallel");
    return 0;
}