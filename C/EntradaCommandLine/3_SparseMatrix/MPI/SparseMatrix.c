#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

/* -----------------------------------------------------------------------------
 * Programa: SparseMatrix&MPI
 * --------------------------
 * Determina si una matriz generada aleatoriamente es dispersa (sparse).
 * Una matriz es sparse si la cantidad de ceros > 50% de sus elementos.
 * 
 * Paraleliza el conteo de ceros usando MPI de manera eficiente:
 * - Se divide la matriz por filas entre los procesos usando MPI_Scatterv.
 * - Cada proceso cuenta ceros localmente.
 * - Se suma el total de ceros con MPI_Reduce.
 * -----------------------------------------------------------------------------
 */
int main(int argc, char** argv) {
    int n, m;        // filas y columnas de la matriz
    int rank, size;  // rank del proceso y cantidad total de procesos

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int *matrix = NULL; // matriz 1D (solo proceso 0)

    // -------------------------------
    // Proceso maestro: pedir dimensiones y generar matriz
    // -------------------------------
    if (rank == 0) {
        printf("--------- SparseMatrix&MPI (CommandLine - explicit data) --------\n");
        if (argc < 3) {
            fprintf(stderr, "Usage: mpirun -np <procs> %s <n> <m> <a00> <a01> ...\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        n = atoi(argv[1]);
        m = atoi(argv[2]);

        if (argc < 3 + n * m) {
            fprintf(stderr, "Error: se esperaban %d valores para la matriz (n*m)\n", n * m);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        matrix = (int*)malloc(n * m * sizeof(int));
        if (!matrix) {
            perror("Error al asignar memoria");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        printf("\nLa matriz proporcionada es:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                matrix[i * m + j] = atoi(argv[3 + i * m + j]);
                printf("%d ", matrix[i * m + j]);
            }
            printf("\n");
        }
    }

    // -------------------------------
    // Difundir dimensiones a todos los procesos
    // -------------------------------
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int total_elements = n * m;

    // -------------------------------
    // Determinar cuántas filas recibirá cada proceso
    // -------------------------------
    int rows_per_proc = n / size;
    int remainder = n % size;
    int my_rows = rows_per_proc + (rank < remainder ? 1 : 0);

    // Reservar memoria local para las filas asignadas
    int *local_matrix = (int*)malloc(my_rows * m * sizeof(int));

    // -------------------------------
    // Preparar datos para MPI_Scatterv
    // sendcounts: cantidad de elementos por proceso
    // displs: desplazamiento de cada proceso en el vector 1D
    // -------------------------------
    int *sendcounts = NULL;
    int *displs = NULL;

    if (rank == 0) {
        sendcounts = (int*)malloc(size * sizeof(int));
        displs = (int*)malloc(size * sizeof(int));

        int offset = 0;
        for (int i = 0; i < size; i++) {
            sendcounts[i] = (rows_per_proc + (i < remainder ? 1 : 0)) * m;
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // -------------------------------
    // Distribuir bloques de filas a todos los procesos
    // -------------------------------
    MPI_Scatterv(matrix, sendcounts, displs, MPI_INT,
                 local_matrix, my_rows * m, MPI_INT,
                 0, MPI_COMM_WORLD);

    // -------------------------------
    // Cada proceso cuenta sus ceros
    // -------------------------------
    int local_zeros = 0;
    double start = MPI_Wtime();

    for (int i = 0; i < my_rows * m; i++) {
        if (local_matrix[i] == 0)
            local_zeros++;
    }

    // -------------------------------
    // Reducir conteos locales a total de ceros en proceso 0
    // -------------------------------
    int total_zeros = 0;
    MPI_Reduce(&local_zeros, &total_zeros, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    // -------------------------------
    // Proceso maestro muestra resultados
    // -------------------------------
    if (rank == 0) {
        printf("\nCantidad total de ceros: %d de %d elementos\n", total_zeros, total_elements);
        if (total_zeros > total_elements / 2)
            printf("La matriz es dispersa (sparse)\n");
        else
            printf("La matriz no es dispersa\n");

        printf("Tiempo total de ejecución (MPI_Wtime): %.6f segundos\n", end - start);

        free(matrix);
        free(sendcounts);
        free(displs);
        printf("---------- Program End ----------\n");
    }

    free(local_matrix);
    MPI_Finalize();
    return 0;
}
