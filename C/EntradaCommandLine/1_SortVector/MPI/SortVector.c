/**
 * ============================================================================
 *  Archivo: SortVector.c
 *
 *  Descripción:
 *      Este programa utiliza el estándar MPI (Message Passing Interface) para
 *      paralelizar el proceso de ordenamiento de un vector de enteros aleatorios
 *      utilizando el algoritmo QuickSort. Cada proceso ordena una parte del vector
 *      y luego los resultados parciales se fusionan secuencialmente en el proceso raíz.
 *
 *  Objetivos:
 *      - Implementar un ordenamiento distribuido utilizando MPI.
 *      - Comparar el rendimiento con la versión secuencial (QuickSort clásico).
 *      - Medir el tiempo de ejecución paralelo mediante MPI_Wtime().
 *
 *  Librerías utilizadas:
 *      <stdio.h>   : Entrada y salida estándar.
 *      <stdlib.h>  : Funciones de memoria dinámica y generación aleatoria.
 *      <time.h>    : Inicialización de la semilla de aleatoriedad.
 *      <mpi.h>     : Biblioteca de paso de mensajes (Message Passing Interface).
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// -----------------------------------------------------------------------------
// Función: swap
// Intercambia dos elementos enteros utilizando punteros.
// -----------------------------------------------------------------------------
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// -----------------------------------------------------------------------------
// Función: partition
// Realiza la partición del arreglo para QuickSort usando el último elemento
// como pivote. Los menores al pivote quedan a la izquierda, los mayores a la derecha.
// -----------------------------------------------------------------------------
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// -----------------------------------------------------------------------------
// Función: quickSort
// Implementa el algoritmo QuickSort recursivo para ordenar un arreglo local.
// -----------------------------------------------------------------------------
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// -----------------------------------------------------------------------------
// Función: merge
// Fusiona dos subarreglos ordenados en un único arreglo ordenado.
// -----------------------------------------------------------------------------
void merge(int* arr, int* temp, int left, int mid, int right) {
    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i < mid) temp[k++] = arr[i++];
    while (j < right) temp[k++] = arr[j++];
    for (i = left; i < right; i++) arr[i] = temp[i];
}

// -----------------------------------------------------------------------------
// Función: main
// Proceso principal del programa MPI.
// - Inicializa MPI.
// - El proceso raíz genera un vector aleatorio.
// - Divide el vector entre procesos (MPI_Scatter).
// - Cada proceso ordena su porción con QuickSort.
// - Se recolectan los resultados (MPI_Gather).
// - El proceso raíz fusiona los subarreglos ordenados.
// -----------------------------------------------------------------------------
int main(int argc, char** argv) {
    int npr, pid;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    int size;
    int* vector = NULL;
    double start, end;

    if (pid == 0) {
        if (argc < 2) {
            fprintf(stderr, "Usage: mpirun -np <procs> %s <size> <v0> <v1> ...\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("--------- SortVector&Parallel (MPI - CommandLine - explicit data) --------\n");
        size = atoi(argv[1]);

        if (argc < 2 + size) {
            fprintf(stderr, "Error: se esperaban %d elementos del vector tras el tamaño\n", size);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Reservar memoria para el vector principal
        vector = (int*)malloc(size * sizeof(int));

        // Leer valores proporcionados por línea de comandos
        printf("\nEl vector proporcionado: [ ");
        for (int i = 0; i < size; i++) {
            vector[i] = atoi(argv[2 + i]);
            printf("%d ", vector[i]);
        }
        printf("]\n");
    }

    // Broadcast del tamaño a todos los procesos
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Preparar distribución variable usando Scatterv (soporta particiones desiguales)
    int *sendcounts = NULL;
    int *displs = NULL;
    int local_size = 0;

    if (pid == 0) {
        sendcounts = malloc(npr * sizeof(int));
        displs = malloc(npr * sizeof(int));
        int base = size / npr;
        int rem = size % npr;
        int offset = 0;
        for (int i = 0; i < npr; i++) {
            sendcounts[i] = base + (i < rem ? 1 : 0);
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // Cada proceso recibe la cantidad de elementos que le tocará
    MPI_Scatter(sendcounts, 1, MPI_INT, &local_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int* local_arr = (int*)malloc(local_size * sizeof(int));

    // Distribución de datos con Scatterv
    MPI_Scatterv(vector, sendcounts, displs, MPI_INT, local_arr, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Medir el tiempo solo una vez en el proceso raíz
    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    // Ordenar localmente cada subarreglo
    quickSort(local_arr, 0, local_size - 1);

    // Recolectar resultados en el proceso raíz
    int* sorted = NULL;
    if (pid == 0) {
        sorted = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(local_arr, local_size, MPI_INT, sorted, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Fusión final secuencial en el proceso raíz
    if (pid == 0) {
        int* temp = (int*)malloc(size * sizeof(int));

        // Fusiona bloques ordenados provenientes de cada proceso
        for (int step = local_size; step < size; step *= 2) {
            for (int i = 0; i < size; i += 2 * step) {
                int mid = i + step;
                int right = i + 2 * step;
                if (mid < size) {
                    if (right > size) right = size;
                    merge(sorted, temp, i, mid, right);
                }
            }
        }

        end = MPI_Wtime();

        // Imprimir el vector final ordenado
        printf("\nVector final ordenado: [ ");
        for (int i = 0; i < size; i++) {
            printf("%d ", sorted[i]);
        }
        printf("]\n");

        printf("\nTiempo total de ejecución (MPI): %.6f segundos\n", end - start);
        printf("---------- Program End ----------\n");

        free(vector);
        free(sorted);
        free(temp);
    }

    free(local_arr);
    MPI_Finalize();
    return 0;
}
