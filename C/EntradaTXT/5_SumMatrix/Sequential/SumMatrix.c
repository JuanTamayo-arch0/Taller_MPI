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

int main() {
    int n, m;

    printf("--------- SumMatrix&Sequential (Parametros) --------\n");

    FILE* f = fopen("params.txt", "r");
    if (!f) {
        fprintf(stderr, "params.txt no encontrado. Coloque un archivo con n m y los elementos A y B en esta carpeta.\n");
        return 1;
    }
    if (fscanf(f, "%d %d", &n, &m) != 2) {
        fprintf(stderr, "Formato de params.txt inválido. Debe contener dos enteros: n m.\n");
        fclose(f);
        return 1;
    }

    // Declarar matrices dinámicamente con dimensiones n x m
    int matrixA[n][m];
    int matrixB[n][m];
    int sum[n][m];

    // Leer matriz A desde params.txt (n*m elementos)
    printf("\nMatriz A leida de params.txt:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (fscanf(f, "%d", &matrixA[i][j]) != 1) {
                fprintf(stderr, "params.txt no contiene suficientes elementos para A (se esperaban %d).\n", n * m);
                fclose(f);
                return 1;
            }
        }
    }
    printMatrix(n, m, matrixA);

    // Leer matriz B desde params.txt (n*m elementos)
    printf("\nMatriz B leida de params.txt:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (fscanf(f, "%d", &matrixB[i][j]) != 1) {
                fprintf(stderr, "params.txt no contiene suficientes elementos para B (se esperaban %d).\n", n * m);
                fclose(f);
                return 1;
            }
        }
    }
    printMatrix(n, m, matrixB);
    fclose(f);

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
