#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Función para imprimir una matriz
void printMatrix(int n, int m, int matrix[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Función para sumar dos matrices
void sumMatrices(int n, int m, int A[n][m], int B[n][m], int result[n][m]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            result[i][j] = A[i][j] + B[i][j];
}

int main() {
    srand(time(NULL));

    int n, m;

    printf("--------- SumMatrix&Sequential --------\n");

    printf("Ingrese el número de filas de las matrices: ");
    scanf("%d", &n);
    printf("Ingrese el número de columnas de las matrices: ");
    scanf("%d", &m);

    int matrixA[n][m];
    int matrixB[n][m];
    int sum[n][m];

    // Generar matrices aleatorias
    printf("\nMatriz A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrixA[i][j] = rand() % 10;

    printMatrix(n, m, matrixA);

    printf("\nMatriz B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            matrixB[i][j] = rand() % 10;

    printMatrix(n, m, matrixB);

    // Medir tiempo de la suma
    clock_t start = clock();
    sumMatrices(n, m, matrixA, matrixB, sum);
    clock_t end = clock();

    printf("\nSuma de A + B:\n");
    printMatrix(n, m, sum);

    double tiempo = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTiempo de ejecución de la suma: %.6f segundos\n", tiempo);

    printf("---------- Program End ----------\n");
    return 0;
}

