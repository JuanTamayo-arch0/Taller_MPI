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

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <size>\n", argv[0]);
        return 1;
    }

    int size = atoi(argv[1]);
    printf("--------- RemoveDuplicates&Sequential (CommandLine - explicit data) --------\n");

    // Comprobamos que se pasen los elementos del vector
    if (argc < 2 + size) {
        fprintf(stderr, "Usage: %s <size> <v0> <v1> ... <v(size-1)>\n", argv[0]);
        return 1;
    }

    // Asignación dinámica de memoria para el vector
    int *vector = malloc(size * sizeof(int));
    if (!vector) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }
    // Leer valores provistos por línea de comandos
    printf("\nEl vector proporcionado es: [ ");
    for (int i = 0; i < size; i++) {
        vector[i] = atoi(argv[2 + i]);
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
