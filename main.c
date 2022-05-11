#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "functions.h"

int main(int argc, char *argv[])
{
    int size, halfSize, rank, numProcs, position = 0, maxIterations = 0;
    char buff[BUFFER_SIZE];

    maxIterations = atoi(argv[1]);

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcs);

    if (rank == MASTER)
    {
        double sumMaster = 0, sumFromSlave = 0,*numbers,t1, t2;

        checkCorrectNumProcess(numProcs);

        numbers = readNumbers(&size);
        halfSize = size / 2;

        MPI_Pack(&halfSize, 1, MPI_INT, buff, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(numbers + halfSize + size % 2, halfSize, MPI_DOUBLE, buff, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Send(buff, position, MPI_PACKED, WORKER, 0, MPI_COMM_WORLD);

        t1 = omp_get_wtime();
        sumMaster = getSumSubArray(halfSize + size % 2, numbers, maxIterations);

        MPI_Recv(&sumFromSlave, 1, MPI_DOUBLE, WORKER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        t2 = omp_get_wtime();

        printf("The total sum is: %f, and it's take: %f sec\n", (sumMaster + sumFromSlave), (t2 - t1));

        free(numbers);
    }
    else
    {

        double sumSlave = 0,*workerArray; 

        MPI_Recv(buff, BUFFER_SIZE, MPI_PACKED, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Unpack(buff, BUFFER_SIZE, &position, &halfSize, 1, MPI_INT, MPI_COMM_WORLD);

        workerArray = (double *)doMalloc(halfSize * sizeof(double));

        if (!workerArray)
        {
            fprintf(stderr, "malloc failed\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        MPI_Unpack(buff, BUFFER_SIZE, &position, workerArray, halfSize, MPI_DOUBLE, MPI_COMM_WORLD);

        sumSlave = getSumSubArray(halfSize, workerArray, maxIterations);
        MPI_Send(&sumSlave, 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);
        
        free(workerArray);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
