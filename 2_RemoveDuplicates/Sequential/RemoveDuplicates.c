#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

void remove_element(int *array, int index, int *size) {
    for (int i = index; i < *size - 1; i++)
        array[i] = array[i + 1];
    (*size)--;
    array = realloc(array, (*size) * sizeof(int));
}

int main() {
    srand(time(NULL));

    int size;
    printf("--------- RemoveDuplicates&Sequential --------\n");
    printf("Ingrese la cantidad de elementos del vector: ");
    scanf("%d", &size);

    int *vector = malloc(size * sizeof(int));
    if (!vector) {
        perror("Error al asignar memoria");
        exit(EXIT_FAILURE);
    }

    printf("\nEl vector elegido al azar es: [ ");
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 101;
        printf("%d ", vector[i]);
    }
    printf("]\n");

    int SequentialTime = clock();

    // Eliminar duplicados
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; ) {
            if (vector[i] == vector[j]) {
                remove_element(vector, j, &size);
            } else {
                j++;
            }
        }
    }

    SequentialTime = clock() - SequentialTime;

    printf("\nEl vector sin duplicados es: [ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", vector[i]);
    }
    printf("]\n");

    printf("\nY se demoro un total de %f segundos\n", (float)SequentialTime / CLOCKS_PER_SEC);
    printf("---------- Program End ----------\n");

    free(vector);
    return 0;
}

