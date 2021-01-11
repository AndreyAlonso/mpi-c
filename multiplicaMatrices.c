/*
-Materia: Supercomputo
-Semestre: 2019-2020 II
-Nombre del alumno: Hector Andrey Hernández Alonso
-Clave del alumno: 249757
-Carrera: Ingeniería Informática
-Nombre de tarea o programa: Programa MPI Multiplicación de matrices
-Periodo de evaluación (parcial 1, 2, 3, 4 o 5): 3
-Descripción: Utilizando funciones de MPI_Scatterv se implemento la multiplicación de matrices
-Avance logrado: (0 a 100%) 100%
-Comentarios adicionales: 
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include "mpi.h"

// Columna y renglon de la matriz 1
#define C1 4
#define R1 3

// Columna y renglon de la matriz 2
#define C2 4
#define R2 3

int *creaArreglo(int tam);
int *generaFila(int *matrizA, int tam, int indice);
int *generaColumna(int *matrizA, int tam, int indice, int columnas);
int *obtenRcounts(int numProc, int nDatos, int nDatosU);
int *obtenDispls(int numProc, int nDatos);
void llenaMatriz(int *matriz, int filas, int columnas);
void imprimeMatriz(int filas, int columnas, int *matriz);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int idProc, numProc, nDatos, nDatosU;
    int *auxR, *rcounts, *displs, *matrizA, *matrizB, *matrizR;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);


    nDatos = (C1 * R2) / (numProc);
    nDatosU = nDatos + (C1 * R2) % (numProc - 1);

    matrizA = creaArreglo(C1 * R1);
    matrizB = creaArreglo(C2 * R2);

    rcounts = obtenRcounts(numProc, nDatos, nDatosU);
    displs = obtenDispls(numProc, nDatos);

    auxR = creaArreglo(nDatos);

    if (idProc == 0)
    {
        llenaMatriz(matrizA, C1, R1);
        llenaMatriz(matrizB, C2, R2);

        imprimeMatriz(C1, R1, matrizA);
        imprimeMatriz(C2, R2, matrizB);
    }
    MPI_Bcast(matrizA, C1 * R1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(matrizB, C2 * R2, MPI_INT, 0, MPI_COMM_WORLD);

    if (idProc == 0)
    {
        matrizR = creaArreglo(C1 * R2);
    }

    int *filas, *columnas, i, j, k;

    filas = creaArreglo(C1);
    columnas = creaArreglo(R2);

    i = (idProc * nDatos) / R2;
    j = (idProc * nDatos) % R2;

    if (idProc == numProc - 1)
    {
        nDatos = nDatosU;
    }
    for (k = 0; k < nDatos; k++)
    {
        filas = generaFila(matrizA, R1, i);
        columnas = generaColumna(matrizB, C2, j, R2);
        int i, resultado;
        resultado = 0;
        for (i = 0; i < C1; i++)
        {
            resultado += *(filas + i) * *(columnas + i);
        }

        auxR[k] = resultado;

        if (j + 1 < R2)
            j++;
        else
        {
            j = 0;
            i++;
        }
    }
    MPI_Gatherv(auxR, nDatos, MPI_INT, matrizR, rcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);
    if (idProc == 0)
    {
        imprimeMatriz(C1, R2, matrizR);
    }
    MPI_Finalize();
    return 0;
}

int *obtenRcounts(int numProc, int nDatos, int nDatosU)
{
    int *rcounts = creaArreglo(numProc);
    int i;
    for (i = 0; i < numProc - 1; i++)
    {
        *(rcounts + i) = nDatos;
    }
    *(rcounts + i) = nDatosU;
    return rcounts;
}

int *obtenDispls(int numProc, int nDatos)
{
    int *displs = creaArreglo(numProc);
    int i;
    for (i = 0; i < numProc; i++)
    {
        *(displs + i) = i * nDatos;
    }
    return displs;
}

int *generaFila(int *matrizA, int tam, int indice)
{
    int i, *fila;
    fila = creaArreglo(tam);
    for (i = 0; i < tam; i++)
    {
        *(fila + i) = *(matrizA + (indice * tam) + i);
    }

    return fila;
}

/**
 * generaColumna esta funcion permite generar la columna para la matriz
 * @matrizA: matriz a llenar
 * @tam: tamaño de la columna
 * @indice: posiicion del arreglo
 * @columnas: cantidad de columnas de  la matriz
 */
int *generaColumna(int *matrizA, int tam, int indice, int columnas)
{
    int i, *columna;
    columna = creaArreglo(tam);
    for (i = 0; i < tam; i++)
    {
        *(columna + i) = *(matrizA + indice + (i * columnas));
    }
    return columna;
}

/**
 * creaArreglo esta funcion reserva espacio de memoria  para el  arreglo
 * @tam: tamaño del arreglo
 */
int *creaArreglo(int tam)
{
    int *aux = (int *)malloc(sizeof(int) * tam);
    return aux;
}

/**
 * llenaMatriz esta funcion asigna valores  a la matriz
 * @matriz: matriz a llenar
 * @filas: numero de  filas de la matriz
 * @columnas: numero de columnas de la matriz
 */
void llenaMatriz(int *matriz, int filas, int columnas)
{
    int i;
    for (i = 0; i < filas * columnas; i++)
    {
        int num = rand() % 9;
        matriz[i] = num;
    }
}

/**
 * imprimeMatriz esta funcion muestra en  pantalla el contenido de la matriz
 * @filas: numero de filas de la matriz
 * @columnas: numero de columnas de la matriz
 * @matriz: matriz a imprimir
 */
void imprimeMatriz(int filas, int columnas, int *matriz)
{
    int i;
    for (i = 0; i < filas * columnas; i++)
    {
        printf("[%d] ", matriz[i]);
        if ((i + 1) % columnas == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}
