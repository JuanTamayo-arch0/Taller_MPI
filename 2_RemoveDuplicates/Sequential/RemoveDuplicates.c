/**
 * ============================================================================
 *  Archivo: RemoveDuplicatesSequential.c
 *
 *  Descripción:
 *      Este programa genera un vector aleatorio de enteros y elimina los valores
 *      duplicados de forma secuencial. Mide el tiempo de ejecución del proceso.
 *
 *  Objetivos:
 *      - Practicar manipulación de memoria dinámica en C.
 *      - Implementar un algoritmo de eliminación de duplicados.
 *      - Calcular el tiempo total de ejecución con clock().
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

// -----------------------------------------------------------------------------
// Función: remove_element
// Descripción: Elimina el elemento en la posición 'index' del arreglo dinámico.
// Parámetros:
//     array : puntero al arreglo de enteros.
//     index : posición del elemento a eliminar.
//     size  : puntero al tamaño actual del arreglo (modificado por referencia).
// -----------------------------------------------------------------------------
void remove_element(int *array, int index, int *size) {
    for (int i = index; i < *size - 1; i++) {
        array[i] = array[i + 1];
    }
    (*size)--; // Reducir tamaño lógico (no realocar aún)
}

int main() {
    srand(time(NULL));

    int size;
    printf("--------- RemoveDuplicates&Sequential --------\n");
    printf("Ingrese la cantidad de elementos del vector: ");
    scanf("%d", &size);

    // Asignación dinámica de memoria para el vector
    int *vector = malloc(size * sizeof(int));
    if (!vector) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    // Generar valores aleatorios en el vector
    printf("\nEl vector generado al azar es: [ ");
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 101;  // Números entre 0 y 100
        printf("%d ", vector[i]);
    }
    printf("]\n");

    clock_t start = clock();

    // -------------------------------------------------------------------------
    // Eliminación de duplicados:
    // Se compara cada elemento con los siguientes y elimina los iguales.
    // Complejidad: O(n²)
    // -------------------------------------------------------------------------
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; ) {
            if (vector[i] == vector[j]) {
                remove_element(vector, j, &size);
            } else {
                j++;
            }
        }
    }

    // Realocación final del arreglo para liberar memoria no usada
    int *tmp = realloc(vector, size * sizeof(int));
    if (tmp != NULL) {
        vector = tmp;
    }

    clock_t end = clock();

    // Mostrar resultados
    printf("\nEl vector sin duplicados es: [ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("]\n");

    printf("\nTiempo total de ejecución: %.6f segundos\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("---------- Program End ----------\n");

    free(vector);
    return 0;
}
