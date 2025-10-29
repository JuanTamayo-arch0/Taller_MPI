#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int n, m;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank); // semilla diferente por proceso

    int *matrix = NULL; // solo el proceso 0 la tendrá

    if (rank == 0) {
        printf("--------- SparseMatrix&MPI --------\n");
        printf("Ingrese la cantidad de filas de la matriz: ");
        scanf("%d", &n);
        printf("Ingrese la cantidad de columnas de la matriz: ");
        scanf("%d", &m);

        matrix = (int*)malloc(n * m * sizeof(int));

        printf("\nLa matriz generada al azar es:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                matrix[i * m + j] = rand() % 2;
                printf("%d ", matrix[i * m + j]);
            }
            printf("\n");
        }
    }

    // Enviar dimensiones a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows_per_proc = n / size;
    int remainder = n % size;

    // Calcular cuántas filas le corresponden a este proceso
    int my_rows = rows_per_proc + (rank < remainder ? 1 : 0);
    int *local_matrix = (int*)malloc(my_rows * m * sizeof(int));

    // Distributir la matriz entre procesos
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

    MPI_Scatterv(matrix, sendcounts, displs, MPI_INT,
                 local_matrix, my_rows * m, MPI_INT,
                 0, MPI_COMM_WORLD);

    // Cada proceso cuenta sus ceros
    int local_zeros = 0;
    double start = MPI_Wtime();

    for (int i = 0; i < my_rows * m; i++) {
        if (local_matrix[i] == 0)
            local_zeros++;
    }

    // Reducir los conteos parciales
    int total_zeros = 0;
    MPI_Reduce(&local_zeros, &total_zeros, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("\nCantidad total de ceros: %d de %d elementos\n", total_zeros, n * m);
        if (total_zeros > (n * m) / 2)
            printf("La matriz es dispersa (sparse)\n");
        else
            printf("La matriz no es dispersa\n");

        printf("Tiempo total de ejecución: %.6f segundos\n", end - start);
        printf("---------- Program End ----------\n");
    }

    free(local_matrix);
    if (rank == 0) {
        free(matrix);
        free(sendcounts);
        free(displs);
    }

    MPI_Finalize();
    return 0;
}
