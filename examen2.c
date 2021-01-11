/********************************************
 *
 * Actividad Supercomputo: Examen 2
 *          Criba de Eratóstenes
 *             PSEUDOCÓDIGO
 * 
 * Hector Andrey Hernández Alonso, 249757
 *
 ********************************************/

N = 100
// identificador del proceso
idProc <= Id Proceso
// numero total de procesos
numProc <= Num. Procesos

Si idProc = 0

    crearArreglo(a, N)
    // arreglo contiene 1's y 0's que ayudara a saber si un numero es primo o no
    // el arreglo se inicializa con 1's
    llenarArreglo(a, N)
    a[0] = 0
    a[1] = 0

    // Recorrer el arreglo de numeros
    Desde i = 2 hasta N 
        Si a[i] == 1 entonces
            // Recorrer los procesos para enviar los arreglos
            Desde iProc = 1 hasta numProc - 1
                // Se envia el arreglo a 
                enviar(iProc, a, N, int)
                // Se envia la posición en que se encuentra el arreglo
                enviar(iProc, i, 1, int)
                i = i + 1
                // Se reciben el arreglo que indica la posicion de los que son primos o no
                recibir(iProc, a, N, int)
                llenarArreglo(a, N)
            Fin Desde
        Fin Si
    Fin Desde

    // Se imprimen los numeros  primos
    Desde i = 0 hasta N-1
        Si a[i] == 1 entonces
            imprime(i)
        Fin Si
    Fin Desde

SiNo

    crearArreglo(a, N)
    pos = 0

    // recibe arreglo 
    recibir(0, a, N, int)
    // recibe la posición en que se encuentra
    recibir(0, pos, 1, int)

    j = 2
    Mientras (j *pos) < N entonces
            a[j*pos] = 0
            j += 1
    Fin Mientras

    enviar(0, a, N, int)
Fin Si
