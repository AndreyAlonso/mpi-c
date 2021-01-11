// 
//  Actividad Supercomputo MPI
//  Suma de los elementos de un arreglo
//  
//  Héctor Andrey Hernández Alonso
//

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "mpi.h" 

#define N 5

void creaArreglo(int a[]);
int sumaArreglo(int a[]);

int main(int argc, char *argv[]) {
  int numProc, idProc, nDatos, nDatosU, i;
  int sump = 0, total = 0;
  MPI_Status status;

  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &idProc);
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);

  nDatos = N/(numProc - 1);
  nDatosU = nDatos + N % (numProc - 1);

  srand(time(NULL));

  if(idProc == 0) {
    int a[N];
    creaArreglo(a);
    printf("Arreglo creado\n");

    // se envia los datos a los procesos
    for(i = 1; i < numProc-1; i++){
        MPI_Send(&a[(i-1)*nDatos], nDatos, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    // Se envia al ultimo proceso
    MPI_Send(&a[(numProc-2)*nDatos], nDatosU, MPI_INT, numProc-1, 0, MPI_COMM_WORLD);

    // Se recibe las sumas parciales
    for(i = 1; i <= numProc-1; i++){
        MPI_Recv(&sump, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        total = total + sump;
    }

    printf("\nSuma del arreglo: %d\n", total);
  }
  else { // No es proceso 0
    // Se valida si es  el ultimo proceso
    if(idProc == numProc -1){
        nDatos = nDatosU;
    }

    int a[nDatos], sum = 0;
    // Se recibe el arreglo del proceso 0
    MPI_Recv(a, nDatos, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    for(i = 0; i < nDatos; i++){
        sum += a[i];
        printf("a[%d] = %d\n", i,a[i]);
    }
    printf("Suma parcial: %d\n", sum);

    MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  
  return 0;
}

// creaArreglo funcion que crea un arreglo aleatorio de enteros
void creaArreglo(int a[]){
  int i;
  printf("\n\n");

  for(i = 0; i < N; i++) {
    a[i] = (int)rand()/(int)(RAND_MAX/50);
    printf("[%d] ", a[i]);
  }

  printf("\n");
}

// sumaArreglo funcion que suma todo los elementos del arreglo a
int sumaArreglo(int a[]){
  int i, res = 0;
  for(i = 0; i < N; i++){
    res += a[i];
  }

  return res;
}