/*
-Materia: Supercomputo
-Semestre: 2019-2020 II
-Nombre del alumno: Hector Andrey Hernández Alonso
-Clave del alumno: 249757
-Carrera: Ingeniería Informática
-Nombre de tarea o programa: Programa MPI: Suma de dos arreglos
-Periodo de evaluación (parcial 1, 2, 3, 4 o 5): 3
-Descripción: Se realizo la suma de dos arreglos  usando funcionciones  MPI_Scatter y MPI_Gather
-Avance logrado: (0 a 100%) 100%
-Comentarios adicionales: 
-   Al final  tuve que poner la condicional si es de nuevo proceso 0 para que imprima bien el arreglo.
-   Sin esa condicional, muestra otros datos
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h" 

#define N 8

void llenaArreglo(int *a);
void imprimeArreglo(int *a);

int main(int argc, char *argv[]) {
    int idProc, numProc, i;
    int nDatos;
    int *a, *b;
    int *c;
    

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    nDatos = N/numProc;
    
    int r1[nDatos], r2[nDatos], r3[nDatos];

    if(idProc == 0) {
        printf("\nProceso actual %d\n", idProc);
        srand(time(NULL));

        a = (int*)malloc(sizeof(int) * N);
        b = (int*)malloc(sizeof(int) * N);
        c = (int*)malloc(sizeof(int) * N);
        llenaArreglo(a);
        llenaArreglo(b);

        printf("\nArreglo a: \n");
        imprimeArreglo(a);

        printf("\nArreglo b: \n");
        imprimeArreglo(b);
    }

    MPI_Scatter(a,nDatos, MPI_INT, &r1, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b,nDatos, MPI_INT, &r2, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    
    printf("\nProceso actual %d\n", idProc);
    for(i = 0; i < nDatos; i++){
        r3[i] = r1[i] + r2[i];
        printf("a[] = %d\tb[] = %d\n",r1[i],r2[i]);
    }

    MPI_Gather(&r3, nDatos, MPI_INT, c, nDatos, MPI_INT, 0, MPI_COMM_WORLD); 

    if(idProc == 0) {
        printf("\nArreglo c: \n");
        imprimeArreglo(c);
    }


    MPI_Finalize();
    
    return 0;
}

void llenaArreglo(int *a) {
    int i;
    for(i = 0; i < N; i++) {
        a[i] = (int)rand()/(int)(RAND_MAX/10);
    }
}

void imprimeArreglo(int *a) {
    int i;
    for(i = 0; i < N; i++){
        printf("[%d] ", a[i]);
    }
    printf("\n\n");
}