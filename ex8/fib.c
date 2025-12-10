#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CUTOFF 20

static long long fib_seq(long long n) {
    long long a = 0;
    long long b = 1;
    for (long long i = 0; i < n; ++i) {
        long long tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

static long long fib_task(long long n, long long cutoff) {
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

static long long fib_parallel(long long n, long long cutoff) {
    long long result = 0;
#pragma omp parallel
    {
#pragma omp single
        { result = fib_task(n, cutoff); }
    }
    return result;
}

static long long verify_small_values(long long cutoff) {
    long long expected = 0;
    long long got = 0;
    for (long long i = 0; i <= 10; ++i) {
        expected = fib_seq(i);
        got = fib_parallel(i, cutoff);
        printf("n=%lld: expected %lld, got %lld\n", i, expected, got);

        if (expected != got) {
            fprintf(stderr, "Mismatch at n=%lld: expected %lld, got %lld\n", i,
                    expected, got);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    int n = 40;
    int cutoff = DEFAULT_CUTOFF;

    if (argc > 1) {
        n = atoi(argv[1]);
        if (n < 0) {
            fprintf(stderr, "n must be non-negative\n");
            return -1;
        }
    }
    if (argc > 2) {
        cutoff = atoi(argv[2]);
        if (cutoff < 0) {
            fprintf(stderr, "cutoff must be non-negative\n");
            return -1;
        }
    }

    // verify_small_values(cutoff);

    double start = omp_get_wtime();
    long long result = fib_seq(n);
    double elapsed = omp_get_wtime() - start;

    // double start = omp_get_wtime();
    // long long result = fib_parallel(n, cutoff);
    // double elapsed = omp_get_wtime() - start;

    printf("fib(%d) = %lld (cutoff=%d)\n", n, result, cutoff);
    printf("Elapsed time: %.6f seconds\n", elapsed);

    return 0;
}
