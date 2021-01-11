#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int men[4], nprocs, myrank, i;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == 0)
    {
        for (i = 0; i < 4; i++)
            men[i] = i;
    }
    else
    {
        for (i = 0; i < 4; i++)
            men[i] = 0;
    }
    printf("Antes: %d %d %d %d\n", men[0], men[1], men[2],
           men[3]);
    MPI_Bcast(men, 4, MPI_INTEGER, 0, MPI_COMM_WORLD);
    printf("Después: %d %d %d %d\n", men[0], men[1], men[2],
           men[3]);
    MPI_Finalize();
    return 0;
}