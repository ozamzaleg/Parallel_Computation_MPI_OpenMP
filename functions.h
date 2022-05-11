#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

enum ranks
{
    MASTER,
    WORKER
};
enum buffer
{
    BUFFER_SIZE = 1024 * 1024
};

enum threads
{
    NUM_THREADS = 2
};

double *readNumbers(int *size);
void *doMalloc(unsigned int nbytes);
double getSumSubArray(int halfSize, double *numbers, int maxIterations);
void checkCorrectNumProcess(int numProcs);
void findMax(int maxIterations, double element, double *sum);
#endif