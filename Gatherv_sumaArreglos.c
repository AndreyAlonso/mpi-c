/*
-Materia: Supercomputo
-Semestre: 2019-2020 II
-Nombre del alumno: Hector Andrey Hernández Alonso
-Clave del alumno: 249757
-Carrera: Ingeniería Informática
-Nombre de tarea o programa: Programa MPI: Suma de dos arreglos con MPI_Scatterv y MPI_Gatherv
-Periodo de evaluación (parcial 1, 2, 3, 4 o 5): 3
-Descripción: Se realizo la suma de dos arreglos  usando funcionciones  MPI_Scatterv y MPI_Gatherv
-Avance logrado: (0 a 100%) 100%
-Comentarios adicionales: 
-   Muy parecido a Scatter y Gather, cambia un poco pero funcionalidad muy similar.
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h"

#define N 6

void llenaArreglo(int *a);
void imprimeArreglo(int *a);
int *reservaMemoria();

int main(int argc, char *argv[])
{
    int idProc, numProc, i;
    int nDatos, nDatosU;
    int *a, *b, *c;        // arreglos que se sumaran
    int *displs, *rcounts; // variables de desplazamiento y coun

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    nDatos = N / (numProc - 1);

    int *r1, *r2, *r3;

    displs = reservaMemoria(numProc);
    rcounts = reservaMemoria(numProc);

    if (idProc == 0)
    {
        printf("\nProceso actual %d\n", idProc);
        srand(time(NULL));

        a = reservaMemoria(N);
        b = reservaMemoria(N);
        c = reservaMemoria(N);

        llenaArreglo(a);
        llenaArreglo(b);
        for (i = 0; i < numProc; i++)
        {
            displs[i] = i * nDatos;

            rcounts[i] = nDatos;
        }

        printf("\nArreglo a: \n");
        imprimeArreglo(a);

        printf("\nArreglo b: \n");
        imprimeArreglo(b);
    }

    r1 = reservaMemoria(nDatos);
    r2 = reservaMemoria(nDatos);
    r3 = reservaMemoria(nDatos);

    MPI_Scatterv(a, rcounts, displs, MPI_INT, r1, nDatos, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, rcounts, displs, MPI_INT, r2, nDatos, MPI_INT, 0, MPI_COMM_WORLD);

    printf("\nProceso actual %d\n", idProc);
    for (i = 0; i < nDatos; i++)
    {
        r3[i] = r1[i] + r2[i];
        printf("a[] = %d\tb[] = %d\n", r1[i], r2[i]);
    }

    MPI_Gatherv(r3, nDatos, MPI_INT, c, rcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    if (idProc == 0)
    {
        printf("\nArreglo c: \n");
        imprimeArreglo(c);
    }

    MPI_Finalize();

    return 0;
}

// llenaArreglo genera el contenido del arreglo con numeros aleatorios
void llenaArreglo(int *a)
{
    int i;
    for (i = 0; i < N; i++)
    {
        a[i] = (int)rand() / (int)(RAND_MAX / 10);
    }
}

// imprimeArreglo muestra en pantalla el contenido del arreglo
void imprimeArreglo(int *a)
{
    int i;
    for (i = 0; i < N; i++)
    {
        printf("[%d] ", a[i]);
    }
    printf("\n\n");
}

// reservaMemoria esta funcion reserva la memoria del arreglo
int *reservaMemoria(int n)
{
    int *aux = (int *)malloc(sizeof(int) * n);

    return aux;
}
