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
    srand(time(NULL)); // Inicializar semilla aleatoria

    int n, m;

    printf("--------- SumMatrix&Sequential --------\n");

    // Leer dimensiones de las matrices
    printf("Ingrese el número de filas de las matrices: ");
    scanf("%d", &n);
    printf("Ingrese el número de columnas de las matrices: ");
    scanf("%d", &m);

    // Declarar matrices dinámicamente con dimensiones n x m
    int matrixA[n][m];
    int matrixB[n][m];
    int sum[n][m];

    // Generar matriz A con valores aleatorios [0,9]
    printf("\nMatriz A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrixA[i][j] = rand() % 10;
    printMatrix(n, m, matrixA);

    // Generar matriz B con valores aleatorios [0,9]
    printf("\nMatriz B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrixB[i][j] = rand() % 10;
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
