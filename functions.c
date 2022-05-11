#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"

void checkCorrectNumProcess(int numProcs)
{
    if (numProcs != 2)
    {
        fprintf(stderr, "Run the example with two processes only\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
}

double *readNumbers(int *size)
{
    int i;
    double *numbers;

    scanf("%d", size);
    numbers = (double *)doMalloc(*size * sizeof(double));

    if (numbers == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    for (i = 0; i < *size; i++)
        scanf("%lf", &numbers[i]);

    return numbers;
}

void *doMalloc(unsigned int nbytes)
{
    void *p = malloc(nbytes);

    if (p == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    return p;
}

void findMax(int maxIterations, double element, double *sum)
{
    double maximum = cos(exp(sin(0)));
    double temp;

    for (int k = 1; k < maxIterations; k++)
    {
        temp = cos(exp(sin(element * k)));
        if (maximum < temp)
            maximum = temp;
    }
    *sum += maximum;
}

double getSumSubArray(int halfSize, double *numbers, int maxIterations)
{
    double sum = 0;
#pragma omp parallel for shared(halfSize,maxIterations,numbers) reduction(+:sum) num_threads(NUM_THREADS)
    for (int i = 0; i < halfSize; i++)
        findMax(maxIterations, numbers[i], &sum);
    return sum;
}
