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
    int size;
    printf("--------- RemoveDuplicates&Sequential (Parametros) --------\n");

    FILE* f = fopen("params.txt", "r");
    if (!f) {
        fprintf(stderr, "params.txt no encontrado. Coloque un archivo con el tamaño y los elementos en esta carpeta.\n");
        return 1;
    }
    if (fscanf(f, "%d", &size) != 1) {
        fprintf(stderr, "Formato de params.txt inválido. Debe contener un entero (size).\n");
        fclose(f);
        return 1;
    }

    // Asignación dinámica de memoria para el vector
    int *vector = malloc(size * sizeof(int));
    if (!vector) {
        perror("Error al asignar memoria");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    // Leer valores desde params.txt
    printf("\nEl vector proporcionado en params.txt es: [ ");
    for (int i = 0; i < size; i++) {
        if (fscanf(f, "%d", &vector[i]) != 1) {
            fprintf(stderr, "params.txt no contiene suficientes elementos (se esperaban %d).\n", size);
            fclose(f);
            free(vector);
            return 1;
        }
        printf("%d ", vector[i]);
    }
    printf("]\n");
    fclose(f);

    clock_t start = clock();

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
