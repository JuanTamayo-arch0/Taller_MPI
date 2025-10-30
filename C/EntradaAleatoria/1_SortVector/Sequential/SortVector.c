/**
 * ============================================================================
 *  Archivo: SortVector.c
 *  Descripción:
 *      Este programa genera un vector de números enteros aleatorios y lo ordena
 *      utilizando el algoritmo QuickSort en su versión secuencial. Además,
 *      mide el tiempo total de ejecución del proceso de ordenamiento.
 *
 *  Objetivo:
 *      - Implementar y comprender el funcionamiento del algoritmo QuickSort.
 *      - Calcular el tiempo de ejecución del proceso de ordenamiento.
 *      - Establecer la base para una futura versión paralela usando MPI.
 *
 *  Librerías utilizadas:
 *      <stdio.h>   : Manejo de entrada/salida estándar.
 *      <stdlib.h>  : Funciones de memoria dinámica, control de procesos y rand().
 *      <time.h>    : Medición de tiempo de ejecución con clock().
 *      <stdbool.h> : Uso de valores booleanos (true/false) — aunque aquí no se usa directamente.
 *      <mpi.h>     : Biblioteca del estándar MPI (Message Passing Interface).
 *                    En esta versión no se utiliza, pero se deja incluida para compatibilidad
 *                    con la futura implementación paralela.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h> // Incluido para futura expansión a versión paralela (no se usa en esta versión)

// -----------------------------------------------------------------------------
// Función: swap
// Descripción:
//     Intercambia los valores de dos variables enteras utilizando paso por referencia.
//
// Parámetros:
//     a: puntero al primer valor.
//     b: puntero al segundo valor.
// -----------------------------------------------------------------------------
void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// -----------------------------------------------------------------------------
// Función: partition
// Descripción:
//     Esta función realiza la operación de partición en el algoritmo QuickSort.
//     Selecciona un pivote (último elemento del subarreglo) y organiza los elementos
//     de tal forma que todos los menores al pivote queden a su izquierda y los mayores
//     a su derecha.
//
// Parámetros:
//     arr[] : arreglo a ordenar.
//     low   : índice inicial del subarreglo.
//     high  : índice final del subarreglo.
//
// Retorna:
//     El índice del pivote después de colocarlo en su posición correcta.
// -----------------------------------------------------------------------------
int partition(int arr[], int low, int high) {
    int pivot = arr[high]; // Se toma el último elemento como pivote
    int i = low - 1;       // Índice del elemento más pequeño

    // Recorre el subarreglo reorganizando los elementos según el pivote
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++; // Incrementa el índice del elemento más pequeño
            swap(&arr[i], &arr[j]);
        }
    }

    // Coloca el pivote en su posición final
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

// -----------------------------------------------------------------------------
// Función: quickSort
// Descripción:
//     Implementa el algoritmo recursivo QuickSort.
//     Divide el arreglo en dos subarreglos a partir del pivote y
//     ordena cada parte recursivamente.
//
// Parámetros:
//     arr[] : arreglo a ordenar.
//     low   : índice inicial del subarreglo.
//     high  : índice final del subarreglo.
// -----------------------------------------------------------------------------
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        // Índice del pivote tras la partición
        int pi = partition(arr, low, high);

        // Ordenar recursivamente los subarreglos
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// -----------------------------------------------------------------------------
// Función principal: main
// Descripción:
//     - Solicita al usuario el tamaño del vector.
//     - Genera números aleatorios en el rango [0,100].
//     - Muestra el vector original.
//     - Ordena el vector usando QuickSort.
//     - Mide y muestra el tiempo total de ejecución.
// -----------------------------------------------------------------------------
int main() {
    srand(time(NULL)); // Inicializa el generador de números aleatorios con la hora actual

    int size; // Tamaño del vector ingresado por el usuario

    printf("--------- SortVector&Sequential --------\n");
    printf("Ingrese la cantidad de elementos del vector: ");
    scanf("%d", &size);

    // Se declara el vector con tamaño dinámico
    int vector[size];

    // ---------------- Generación de números aleatorios -----------------
    printf("\nEl vector elegido al azar es: [ ");
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 101; // Genera números entre 0 y 100
        printf("%d ", vector[i]);
    }
    printf("]\n");

    // ---------------- Proceso de ordenamiento -----------------
    printf("\nEl vector ordenado es: [ ");
    clock_t SequentialTime = clock(); // Inicio del conteo de tiempo

    quickSort(vector, 0, size - 1);   // Llamada al algoritmo de ordenamiento

    SequentialTime = clock() - SequentialTime; // Fin del conteo de tiempo

    // ---------------- Impresión del resultado -----------------
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("]\n");

    // ---------------- Tiempo total de ejecución -----------------
    printf("\nTiempo total de ejecución: %f segundos",
           (float)SequentialTime / CLOCKS_PER_SEC);

    printf("\n---------- Program End ----------\n");

    return 0;
}
