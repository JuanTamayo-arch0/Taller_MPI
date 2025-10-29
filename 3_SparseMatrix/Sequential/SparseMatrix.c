#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main() {
    srand(time(NULL));

    int n, m;
    int zeros = 0;

    printf("--------- SparseMatrix&Sequential --------\n");

    printf("Ingrese la cantidad de filas de la matriz: ");
    scanf("%d", &n);

    printf("Ingrese la cantidad de columnas de la matriz: ");
    scanf("%d", &m);

    int matrix[n][m];

    printf("\nLa matriz generada al azar es:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i][j] = rand() % 2;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    clock_t start = clock();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (matrix[i][j] == 0)
                zeros++;
        }
    }

    clock_t end = clock();

    if (zeros > (n * m) / 2)
        printf("\nLa matriz es dispersa (sparse)\n");
    else
        printf("\nLa matriz no es dispersa\n");

    printf("Cantidad de ceros: %d de %d elementos\n", zeros, n * m);
    printf("Tiempo total de ejecución: %.6f segundos\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("---------- Program End ----------\n");
    return 0;
}
