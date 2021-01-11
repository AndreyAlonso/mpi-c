// 
//  Supercomputo MPI
//  Programa 3: MPI_Bcast
//  
//  Héctor Andrey Hernández Alonso
//
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h" 

#define N 5

void llenaArreglo(float *a);
void imprimeArreglo(float *a);

int main(int argc, char *argv[]) {

    int idProc, i, j;
    float *a;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);

    a = (float*)malloc(sizeof(float)*N);
    
    if(idProc == 0) {
        llenaArreglo(a);
        printf("Arreglo creado\n");
    }

    MPI_Bcast(a, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
    
    printf("\nProceso actual %d\n", idProc);
    imprimeArreglo(a);

    free(a);
    MPI_Finalize();

    return 0;
}

void llenaArreglo(float *a) {
    int i;
    for(i = 0; i < N; i++) {
        a[i] = (float)rand()/(float)(RAND_MAX/50);
    
    }
}

void imprimeArreglo(float *a) {
    int i;
    for(i = 0; i < N; i++){
        printf("[%.2f] ", a[i]);
    }
    printf("\n\n");
}
