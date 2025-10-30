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
    int n, m; // filas y columnas

    printf("--------- SymmetricMatrix&Sequential (Parametros) --------\n");

    FILE* f = fopen("params.txt", "r");
    if (!f) {
        fprintf(stderr, "params.txt no encontrado. Coloque un archivo con n y m y los elementos en esta carpeta.\n");
        return 1;
    }
    if (fscanf(f, "%d %d", &n, &m) != 2) {
        fprintf(stderr, "Formato de params.txt inválido. Debe contener dos enteros: n m.\n");
        fclose(f);
        return 1;
    }

    // Crear matriz n x m y leer elementos desde params.txt (fila mayor)
    int matrix[n][m];
    printf("\nLa matriz leida de params.txt es:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (fscanf(f, "%d", &matrix[i][j]) != 1) {
                fprintf(stderr, "params.txt no contiene suficientes elementos (se esperaban %d).\n", n * m);
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);

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
