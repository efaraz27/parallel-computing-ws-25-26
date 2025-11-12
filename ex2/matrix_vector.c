// Usage: ./matrix_vector <rows> <cols>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <rows> <cols>\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    if (rows <= 0 || cols <= 0)
    {
        fprintf(stderr, "rows and cols must be positive integers\n");
        return 1;
    }

    double *A = (double *)malloc((size_t)rows * (size_t)cols * sizeof(double));
    double *x = (double *)malloc((size_t)cols * sizeof(double));
    double *y = (double *)malloc((size_t)rows * sizeof(double));
    if (!A || !x || !y)
    {
        fprintf(stderr, "allocation failed\n");
        free(A);
        free(x);
        free(y);
        return 1;
    }

    srand(42);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            A[(size_t)i * (size_t)cols + (size_t)j] = (double)rand() / (double)RAND_MAX;
        }
    }
    for (int j = 0; j < cols; ++j)
    {
        x[j] = (double)rand() / (double)RAND_MAX;
    }

    for (int i = 0; i < rows; ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < cols; ++j)
        {
            sum += A[(size_t)i * (size_t)cols + (size_t)j] * x[j];
        }
        y[i] = sum;
    }

    int to_print = rows < 10 ? rows : 10;
    for (int i = 0; i < to_print; ++i)
    {
        printf("y[%d] = %.6f\n", i, y[i]);
    }

    free(A);
    free(x);
    free(y);
    return 0;
}