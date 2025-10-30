#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

/* -----------------------------------------------------------------------------
 * Programa: SymmetricMatrix&MPI
 * -----------------------------
 * Este programa genera una matriz cuadrada binaria (0 y 1), calcula su 
 * transpuesta y verifica si la matriz es simétrica usando MPI para paralelizar
 * la verificación de filas entre los procesos.
 *
 * Funciones:
 *  - printMatrix: imprime cualquier matriz n x m
 *  - isSymmetricPartial: verifica simetría solo para un rango de filas
 * -----------------------------------------------------------------------------
 */

 void printMatrix(int n, int m, int matrix[n][m]) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Función que verifica simetría solo para un rango de filas [start_row, end_row)
bool isSymmetricPartial(int n, int start_row, int end_row, int matrix[n][n], int transpose[n][n]) {
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != transpose[i][j]) {
                return false; // alguna fila no coincide, no es simétrica
            }
        }
    }
    return true; // todas las filas del rango coinciden
}

int main(int argc, char** argv) {
    int rank, size;

    // Inicialización de MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int n, m;
    int *matrix_data = NULL;     // solo lo tiene el proceso 0
    int *transpose_data = NULL;  // solo lo tiene el proceso 0

    if (rank == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: mpirun -np <procs> %s <n> <m>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("--------- SymmetricMatrix&MPI (CommandLine) --------\n");
        n = atoi(argv[1]);
        m = atoi(argv[2]);

        // Solo matrices cuadradas pueden ser simétricas
        if (n != m) {
            printf("\nLa matriz no es cuadrada, por lo tanto no puede ser simétrica.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        // Reservar memoria y llenar matriz original a partir de argumentos
        matrix_data = malloc(n * m * sizeof(int));
        transpose_data = malloc(n * m * sizeof(int));

        if (argc < 3 + n * m) {
            fprintf(stderr, "Usage: mpirun -np <procs> %s <n> <m> <a00> <a01> ...\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        printf("\nLa matriz proporcionada es:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                matrix_data[i * m + j] = atoi(argv[3 + i * m + j]);
                printf("%d ", matrix_data[i * m + j]);
            }
            printf("\n");
        }

        // Calcular la transpuesta
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                transpose_data[j * m + i] = matrix_data[i * m + j];

        printf("\nLa matriz transpuesta es:\n");
        printMatrix(n, m, (int (*)[m])transpose_data);
    }

    // Compartir dimensiones con todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Reservar espacio en todos los procesos
    int *matrix = malloc(n * m * sizeof(int));
    int *transpose = malloc(n * m * sizeof(int));

    // Enviar matrices a todos los procesos
    MPI_Bcast(matrix_data ? matrix_data : matrix, n * m, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(transpose_data ? transpose_data : transpose, n * m, MPI_INT, 0, MPI_COMM_WORLD);

    // Copiar datos si no es el proceso 0
    if (rank != 0) {
        for (int i = 0; i < n * m; i++) {
            matrix[i] = matrix[i];
            transpose[i] = transpose[i];
        }
    } else {
        for (int i = 0; i < n * m; i++) {
            matrix[i] = matrix_data[i];
            transpose[i] = transpose_data[i];
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // Dividir filas entre procesos
    int rows_per_proc = n / size;
    int remainder = n % size;
    int start_row = rank * rows_per_proc + (rank < remainder ? rank : remainder);
    int end_row = start_row + rows_per_proc + (rank < remainder ? 1 : 0);

    // Verificar simetría local
    bool local_symmetric = isSymmetricPartial(n, start_row, end_row,
                                              (int (*)[n])matrix, (int (*)[n])transpose);
    int local_result = local_symmetric ? 1 : 0;
    int global_result;

    // Reducir resultados: si algún proceso falla, el resultado global será 0
    MPI_Reduce(&local_result, &global_result, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // Proceso maestro imprime el resultado final
    if (rank == 0) {
        printf("\nResultado final:\n");
        if (global_result == 1)
            printf("La matriz es simétrica.\n");
        else
            printf("La matriz no es simétrica.\n");

        printf("Tiempo total de ejecución: %.6f segundos\n", end_time - start_time);
        printf("---------- Program End ----------\n");
    }

    // Liberar memoria
    free(matrix);
    free(transpose);
    if (rank == 0) {
        free(matrix_data);
        free(transpose_data);
    }

    MPI_Finalize();
    return 0;
}
