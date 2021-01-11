/*
-Materia: Supercomputo
-Semestre: 2019-2020 II
-Nombre del alumno: Hector Andrey Hernández Alonso
-Clave del alumno: 249757
-Carrera: Ingeniería Informática
-Nombre de tarea o programa: Programa MPI: Tipo de datos MPI
-Periodo de evaluación (parcial 1, 2, 3, 4 o 5): 3
-Descripción: Se realizo ejercicio con estructuras utilizando MPI_Struct
-Avance logrado: (0 a 100%) 100%
-Comentarios adicionales: 
    - Se uso MPI_Bcast para enviar la estructura de tipos a los demas procesos
    - Se uso MPI_Scatterv para enviar el arreglo de empleados según nDatos
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <string.h>
#include "mpi.h"

typedef struct
{
    int cve_tipo;
    float pago;
    float pago_hora_extra;
} TIPOE;

typedef struct
{
    int cve_empleado;
    char nombre[100];
    int cve_tipo;
    float horas_extra;
    float total_pago;
} EMPLEADO;

void generaEmpleados(EMPLEADO *empleados, int n);
void generaTipos(TIPOE *tipos, int n);
void imprimeEmpleados(EMPLEADO *empleados, int num_empleados);

int main(int argc, char *argv[])
{
    int num_tipos = 3;
    int num_empleados = 1000;
    TIPOE *tipos;
    EMPLEADO *empleadosT;
    float total = 0;
    int no_validos = 0;
    int i, j;

    // Type Struct para Empleado
    MPI_Datatype MPI_EMPLEADO;
    int tamEmpleado[4];
    MPI_Aint despEmpleado[4];
    MPI_Datatype tipoBloqueEmpleado[4];

    tamEmpleado[0] = 1;
    tamEmpleado[1] = 100;
    tamEmpleado[2] = 1;
    tamEmpleado[3] = 2;

    despEmpleado[0] = 0;
    despEmpleado[1] = sizeof(int);
    despEmpleado[2] = sizeof(int) + sizeof(char) * 100;
    despEmpleado[3] = sizeof(int) + sizeof(char) * 100 + sizeof(int);

    tipoBloqueEmpleado[0] = MPI_INT;
    tipoBloqueEmpleado[1] = MPI_CHAR;
    tipoBloqueEmpleado[2] = MPI_INT;
    tipoBloqueEmpleado[3] = MPI_FLOAT;

    // Type Struct para Tipo
    MPI_Datatype MPI_TIPOE;
    int tamTipo[2];
    MPI_Aint despTipo[2];
    MPI_Datatype tipoBloqueTipo[2];

    tamTipo[0] = 1;
    tamTipo[1] = 2;

    despTipo[0] = 0;
    despTipo[1] = sizeof(int);

    tipoBloqueTipo[0] = MPI_INT;
    tipoBloqueTipo[1] = MPI_FLOAT;

    int idProc, numProc;
    int *rcounts, *displs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);

    MPI_Type_struct(2, tamTipo, despTipo, tipoBloqueTipo, &MPI_TIPOE);
    MPI_Type_struct(4, tamEmpleado, despEmpleado, tipoBloqueEmpleado, &MPI_EMPLEADO);
    MPI_Type_commit(&MPI_TIPOE);
    MPI_Type_commit(&MPI_EMPLEADO);

    int nDatos = num_empleados / (numProc - 1);
    int nDatosU = nDatos + num_empleados % (numProc - 1);

    EMPLEADO *empleados;
    empleadosT = (EMPLEADO *)malloc(sizeof(EMPLEADO) * num_empleados);
    tipos = (TIPOE *)malloc(sizeof(TIPOE) * num_tipos);

    displs = (int *)malloc(sizeof(int) * numProc);
    rcounts = (int *)malloc(sizeof(int) * numProc);

    if (idProc == 0)
    {
        generaTipos(tipos, num_tipos);
        generaEmpleados(empleadosT, num_empleados);

        for (i = 0; i < numProc; ++i)
        {
            displs[i] = i * nDatos;
            rcounts[i] = nDatos;
            if (i == (numProc - 1))
            {
                rcounts[i] = nDatosU;
                displs[i] = i * nDatosU;
            }
        }
    }

    if (idProc == (numProc - 1))
    {
        nDatos = nDatosU;
    }
    empleados = (EMPLEADO *)malloc(sizeof(EMPLEADO) * nDatos);

    MPI_Bcast(tipos, num_tipos, MPI_TIPOE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(empleadosT, rcounts, displs, MPI_EMPLEADO, empleados, nDatos, MPI_EMPLEADO, 0, MPI_COMM_WORLD);

    for (i = 0; i < nDatos; i++)
    {
        for (j = 0; j < num_tipos; j++)
        {
            if (empleados[i].cve_tipo == tipos[j].cve_tipo)
            {
                break;
            }
        }
        if (j < num_tipos)
        {
            empleados[i].total_pago = tipos[j].pago + empleados[i].horas_extra * tipos[j].pago_hora_extra;
            total += empleados[i].total_pago;
        }
        else
        {
            empleados[i].total_pago = -1;
            no_validos++;
        }
    }

    MPI_Gatherv(empleados, nDatos, MPI_EMPLEADO, empleadosT, rcounts, displs, MPI_EMPLEADO, 0, MPI_COMM_WORLD);
    if (idProc == 0)
    {
        imprimeEmpleados(empleadosT, num_empleados);
        printf("El total es %.2f, registros no validos: %d\n\n", total, no_validos);
        MPI_Type_free(&MPI_TIPOE);
        MPI_Type_free(&MPI_EMPLEADO);
    }

    MPI_Finalize();
    return 0;
}

/**
 * generaTipos esta funcion asigna los valores al arreglo de tipos
 * @tipos: arreglo de tipos
 * @n: tamaño del arreglo de tipos
 */
void generaTipos(TIPOE *tipos, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        tipos[i].cve_tipo = i + 1;
        tipos[i].pago = i + 1 * 100;
        tipos[i].pago_hora_extra = i + 1 / 2;
    }
}

/**
 * generaEmpleados esta funcion asigna los valores al arreglo de empleados
 * @empleados: arreglo de empleados
 * @n: tamaño del arreglo de empleados
 */
void generaEmpleados(EMPLEADO *empleados, int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        empleados[i].cve_empleado = i + 1;
        strcpy(empleados[i].nombre, "Luis");
        empleados[i].cve_tipo = rand() % 3;
        empleados[i].horas_extra = 2;
        empleados[i].total_pago = n * n;
    }
}

/**
 * imprimeEmpleados esta funcion muestra en pantalla la lista de empleados
 * @empleados: arreglo de empleados
 * @num_empleados: cantidad de empleados que tiene el arreglo
 */
void imprimeEmpleados(EMPLEADO *empleados, int num_empleados)
{
    int i;
    for (i = 0; i < num_empleados; i++)
    {
        printf("\n\nDatos Empleado -----------------------------------\n");
        printf("\tClave empleado:   \t%d \n", empleados[i].cve_empleado);
        printf("\tNombre empleado:  \t%s \n", empleados[i].nombre);
        printf("\tTipo empleado:    \t%d \n", empleados[i].cve_tipo);
        printf("\tHoras extra:      \t%.2f \n", empleados[i].horas_extra);
        printf("\tHoras extra:      \t%.2f \n", empleados[i].total_pago);
    }
}
