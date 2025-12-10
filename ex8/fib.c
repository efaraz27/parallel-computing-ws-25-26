#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CUTOFF 20

static long long fib_seq(int n) {
    long long a = 0;
    long long b = 1;
    for (int i = 0; i < n; ++i) {
        long long tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

static long long fib_task(int n, int cutoff) {
    if (n < 2) {
        return n;
    }
    if (n <= cutoff) {
        return fib_seq(n);
    }

    long long x = 0;
    long long y = 0;

#pragma omp task shared(x) firstprivate(n, cutoff)
    { x = fib_task(n - 1, cutoff); }

#pragma omp task shared(y) firstprivate(n, cutoff)
    { y = fib_task(n - 2, cutoff); }

#pragma omp taskwait
    return x + y;
}

static long long fib_parallel(int n, int cutoff) {
    long long result = 0;
#pragma omp parallel
    {
#pragma omp single
        { result = fib_task(n, cutoff); }
    }
    return result;
}

static int verify_small_values(int cutoff) {
    for (int i = 0; i <= 10; ++i) {
        long long expected = fib_seq(i);
        long long got = fib_parallel(i, cutoff);
        if (expected != got) {
            fprintf(stderr, "Mismatch at n=%d: expected %lld, got %lld\n", i,
                    expected, got);
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    int n = 40;
    int cutoff = DEFAULT_CUTOFF;

    if (argc > 1) {
        n = atoi(argv[1]);
        if (n < 0) {
            fprintf(stderr, "n must be non-negative\n");
            return 1;
        }
    }
    if (argc > 2) {
        cutoff = atoi(argv[2]);
        if (cutoff < 0) {
            fprintf(stderr, "cutoff must be non-negative\n");
            return 1;
        }
    }

    verify_small_values(cutoff)

    // double start = omp_get_wtime();
    // long long result = fib_parallel(n, cutoff);
    // double elapsed = omp_get_wtime() - start;

    // printf("fib(%d) = %lld (cutoff=%d)\n", n, result, cutoff);
    // printf("Elapsed time: %.6f seconds\n", elapsed);

    return 0;
}
