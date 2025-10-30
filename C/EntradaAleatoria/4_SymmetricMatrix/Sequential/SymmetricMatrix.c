#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/* -----------------------------------------------------------------------------
 * Programa: SymmetricMatrix&Sequential
 * --------------------------
 * Este programa genera una matriz aleatoria binaria (0 y 1), la imprime, 
 * calcula su transpuesta y verifica si la matriz es simétrica (solo si es cuadrada).
 * 
 * Funciones:
 *  - printMatrix: imprime una matriz n x m en consola
 *  - compareMatrices: compara dos matrices n x m y retorna true si son iguales
 * -----------------------------------------------------------------------------
 */

// Función para imprimir una matriz
void printMatrix(int n, int m, int matrix[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Función para comparar dos matrices (retorna true si son iguales)
bool compareMatrices(int n, int m, int A[n][m], int B[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (A[i][j] != B[i][j])
                return false;
        }
    }
    return true;
}

int main() {
    srand(time(NULL));

    int n, m; // filas y columnas

    printf("--------- SymmetricMatrix&Sequential --------\n");

    printf("Ingrese la cantidad de filas de la matriz: ");
    scanf("%d", &n);

    printf("Ingrese la cantidad de columnas de la matriz: ");
    scanf("%d", &m);

    // Crear matriz n x m
    int matrix[n][m];

    // Llenar matriz con valores aleatorios 0 o 1
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = rand() % 2; 
        }
    }

    printf("\nLa matriz generada al azar es:\n");
    printMatrix(n, m, matrix);

    clock_t start = clock();

    if (n == m) { // solo tiene sentido si es cuadrada
        int transpose[n][m];

        // Calcular la transpuesta
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                transpose[j][i] = matrix[i][j];
            }
        }

        printf("\nLa matriz transpuesta es:\n");
        printMatrix(n, m, transpose);

        // Verificar si la matriz es simétrica
        if (compareMatrices(n, m, matrix, transpose)) {
            printf("\nLa matriz es simétrica.\n");
        } else {
            printf("\nLa matriz no es simétrica.\n");
        }

    } else {
        printf("\nLa matriz no es cuadrada, por lo tanto no puede ser simétrica.\n");
    }

    clock_t end = clock();

    // Mostrar tiempo de ejecución
    printf("\nTiempo total de ejecución: %.6f segundos\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("---------- Program End ----------\n");
    return 0;
}
