#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* -----------------------------------------------------------------------------
 * Programa: SumMatrix&Sequential
 * ------------------------------
 * Este programa genera dos matrices aleatorias de tamaño n x m y calcula su suma
 * de manera secuencial. Además, imprime las matrices originales y la matriz
 * resultante, y mide el tiempo de ejecución de la operación de suma.
 *
 * Funciones:
 *  - printMatrix: imprime cualquier matriz n x m
 *  - sumMatrices: realiza la suma elemento a elemento de dos matrices n x m
 * -----------------------------------------------------------------------------
 */

// Función para imprimir una matriz n x m
void printMatrix(int n, int m, int matrix[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Función para sumar dos matrices A y B y almacenar el resultado en 'result'
void sumMatrices(int n, int m, int A[n][m], int B[n][m], int result[n][m]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result[i][j] = A[i][j] + B[i][j];
}

int main(int argc, char **argv) {
    // Entrada por línea de comandos; no se usa generación aleatoria aquí.

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    printf("--------- SumMatrix&Sequential (CommandLine - explicit data) --------\n");

    // Comprobar que hay suficientes argumentos: 2 matrices A y B (n*m cada una)
    if (argc < 3 + 2 * n * m) {
        fprintf(stderr, "Usage: %s <n> <m> <A_elements(n*m)> <B_elements(n*m)>\n", argv[0]);
        return 1;
    }

    // Declarar matrices dinámicamente con dimensiones n x m
    int matrixA[n][m];
    int matrixB[n][m];
    int sum[n][m];

    // Leer matriz A desde argv
    printf("\nMatriz A proporcionada:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrixA[i][j] = atoi(argv[3 + i * m + j]);
        }
    }
    printMatrix(n, m, matrixA);

    // Leer matriz B desde argv (después de A)
    printf("\nMatriz B proporcionada:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrixB[i][j] = atoi(argv[3 + n * m + i * m + j]);
        }
    }
    printMatrix(n, m, matrixB);

    // Medir tiempo de ejecución de la suma de matrices
    clock_t start = clock();
    sumMatrices(n, m, matrixA, matrixB, sum);
    clock_t end = clock();

    // Imprimir matriz resultante
    printf("\nSuma de A + B:\n");
    printMatrix(n, m, sum);

    // Calcular y mostrar tiempo de ejecución
    double tiempo = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTiempo de ejecución de la suma: %.6f segundos\n", tiempo);

    printf("---------- Program End ----------\n");
    return 0;
}
