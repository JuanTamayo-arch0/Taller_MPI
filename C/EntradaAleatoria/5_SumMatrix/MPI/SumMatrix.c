#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

/* -----------------------------------------------------------------------------
 * Programa: SumMatrix&MPI
 * -----------------------
 * Este programa realiza la suma de dos matrices de tamaño n x m utilizando MPI
 * para paralelizar la operación por filas. Cada proceso trabaja en un bloque de
 * filas de las matrices y luego los resultados se recopilan en el proceso 0.
 *
 * Funcionalidades:
 *  - Generación de matrices aleatorias
 *  - Distribución de filas entre procesos
 *  - Suma paralela de matrices
 *  - Recolección de resultados con MPI_Gatherv
 *  - Medición del tiempo de ejecución paralelo
 * -----------------------------------------------------------------------------
 */

// Función para imprimir matrices (en procesos locales o globales)
void printMatrix(int rows, int cols, int matrix[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializar MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // ID del proceso actual
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número total de procesos

    int n, m; // Dimensiones de las matrices
    int *A = NULL, *B = NULL, *C = NULL;

    if (rank == 0) {
        srand(time(NULL));

        printf("--------- SumMatrix&MPI --------\n");
        printf("Ingrese el número de filas de las matrices: ");
        scanf("%d", &n);
        printf("Ingrese el número de columnas de las matrices: ");
        scanf("%d", &m);

        // Reservar memoria para las matrices globales
        A = malloc(n * m * sizeof(int));
        B = malloc(n * m * sizeof(int));
        C = malloc(n * m * sizeof(int));

        // Generar matriz A aleatoria y mostrarla
        printf("\nMatriz A:\n");
        for (int i = 0; i < n * m; i++) {
            A[i] = rand() % 10;
            if (i % m == m - 1) printf("%d\n", A[i]);
            else printf("%d ", A[i]);
        }

        // Generar matriz B aleatoria y mostrarla
        printf("\nMatriz B:\n");
        for (int i = 0; i < n * m; i++) {
            B[i] = rand() % 10;
            if (i % m == m - 1) printf("%d\n", B[i]);
            else printf("%d ", B[i]);
        }
    }

    // Compartir dimensiones a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular cuántas filas le toca a cada proceso
    int rows_per_proc = n / size;
    int remainder = n % size;
    int my_rows = rows_per_proc + (rank < remainder ? 1 : 0);

    // Reservar memoria para las porciones locales
    int *local_A = malloc(my_rows * m * sizeof(int));
    int *local_B = malloc(my_rows * m * sizeof(int));
    int *local_C = malloc(my_rows * m * sizeof(int));

    int *sendcounts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        // Preparar sendcounts y desplazamientos para MPI_Scatterv
        sendcounts = malloc(size * sizeof(int));
        displs = malloc(size * sizeof(int));

        int offset = 0;
        for (int i = 0; i < size; i++) {
            int rows = rows_per_proc + (i < remainder ? 1 : 0);
            sendcounts[i] = rows * m; // cantidad de elementos a enviar a cada proceso
            displs[i] = offset;       // desplazamiento en el arreglo global
            offset += sendcounts[i];
        }
    }

    // Distribuir filas de A y B a los procesos
    MPI_Scatterv(A, sendcounts, displs, MPI_INT, local_A, my_rows * m, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(B, sendcounts, displs, MPI_INT, local_B, my_rows * m, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD); // Sincronizar antes de medir tiempo
    double start_time = MPI_Wtime(); // Tiempo paralelo

    // Sumar localmente las filas asignadas
    for (int i = 0; i < my_rows * m; i++)
        local_C[i] = local_A[i] + local_B[i];

    // Recolectar los resultados en C
    MPI_Gatherv(local_C, my_rows * m, MPI_INT, C, sendcounts, displs, MPI_INT, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime(); // Fin del tiempo paralelo

    // Proceso 0 imprime matriz resultante y tiempo
    if (rank == 0) {
        printf("\nMatriz C = A + B:\n");
        for (int i = 0; i < n * m; i++) {
            if (i % m == m - 1) printf("%d\n", C[i]);
            else printf("%d ", C[i]);
        }
        printf("\nTiempo de ejecución paralelo: %.6f segundos\n", end_time - start_time);
        printf("---------- Program End ----------\n");

        // Liberar memoria
        free(A);
        free(B);
        free(C);
        free(sendcounts);
        free(displs);
    }

    // Liberar memoria local
    free(local_A);
    free(local_B);
    free(local_C);

    MPI_Finalize();
    return 0;
}
