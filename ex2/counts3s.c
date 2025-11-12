#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define T 8

int seq_count3s(int *arr, int len);
int omp_variant1(int *arr, int len);
int omp_variant2(int *arr, int len);
int omp_variant3(int *arr, int len);
int omp_variant4(int *arr, int len);

int main()
{
    const int n = 1000000;
    int *arr = (int *)malloc(sizeof(int) * n);
    if (!arr)
    {
        fprintf(stderr, "Allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; ++i)
    {
        arr[i] = rand() % 10;
    }

    double t0, t1;

    t0 = omp_get_wtime();
    int c_seq = seq_count3s(arr, n);
    t1 = omp_get_wtime();
    double dt_seq = t1 - t0;

    t0 = omp_get_wtime();
    int c_v1 = omp_variant1(arr, n);
    t1 = omp_get_wtime();
    double dt_v1 = t1 - t0;

    t0 = omp_get_wtime();
    int c_v2 = omp_variant2(arr, n);
    t1 = omp_get_wtime();
    double dt_v2 = t1 - t0;

    t0 = omp_get_wtime();
    int c_v3 = omp_variant3(arr, n);
    t1 = omp_get_wtime();
    double dt_v3 = t1 - t0;

    t0 = omp_get_wtime();
    int c_v4 = omp_variant4(arr, n);
    t1 = omp_get_wtime();
    double dt_v4 = t1 - t0;

    printf("seq: \tcount=%d \ttime=%.6f s\n", c_seq, dt_seq);
    printf("v1 (reduction): \tcount=%d \ttime=%.6f s\n", c_v1, dt_v1);
    printf("v2 (atomic): \tcount=%d \ttime=%.6f s\n", c_v2, dt_v2);
    printf("v3 (private): \tcount=%d \ttime=%.6f s\n", c_v3, dt_v3);
    printf("v4 (padded): \tcount=%d \ttime=%.6f s\n", c_v4, dt_v4);

    free(arr);
    return 0;
}

int seq_count3s(int *arr, int len)
{
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == 3)
        {
            count++;
        }
    }
    return count;
}

int omp_variant1(int *arr, int len)
{
    int count = 0;
    omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == 3)
        {
            count++;
        }
    }
    return count;
}

int omp_variant2(int *arr, int len)
{
    int count = 0;
    omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < len; i++)
    {
        if (arr[i] == 3)
        {
#pragma omp atomic
            ++count;
        }
    }
    return count;
}

int omp_variant3(int *arr, int len)
{
    int count = 0;
    int private_count[T];
    for (int i = 0; i < T; ++i)
    {
        private_count[i] = 0;
    }
    omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < len; ++i)
    {
        if (arr[i] == 3)
        {
            ++private_count[omp_get_thread_num()];
        }
    }
#pragma omp parallel
    {
#pragma omp atomic
        count += private_count[omp_get_thread_num()];
    }
    return count;
}

int omp_variant4(int *arr, int len)
{
    int count = 0;
    struct padded_int
    {
        int value;
        char padding[60];
    } private_count[T];
    for (int i = 0; i < T; ++i)
    {
        private_count[i].value = 0;
    }
    omp_set_num_threads(T);
#pragma omp parallel for schedule(static)
    for (int i = 0; i < len; ++i)
    {
        if (arr[i] == 3)
        {
            ++private_count[omp_get_thread_num()].value;
        }
    }
#pragma omp parallel
    {
#pragma omp atomic
        count += private_count[omp_get_thread_num()].value;
    }
    return count;
}
