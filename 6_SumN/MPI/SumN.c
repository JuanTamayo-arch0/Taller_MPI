#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;

    if (rank == 0) {
        printf("--------- SumN&MPI --------\n");
        printf("Ingrese la cantidad de los primeros numeros naturales a sumar: ");
        scanf("%d", &n);
    }

    // Enviar n a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Determinar rango de cada proceso
    int numbers_per_proc = n / size;
    int remainder = n % size;

    int start = rank * numbers_per_proc + (rank < remainder ? rank : remainder) + 1;
    int end = start + numbers_per_proc - 1 + (rank < remainder ? 1 : 0);

    // Calcular suma parcial
    int local_sum = 0;
    for (int i = start; i <= end; i++) {
        local_sum += i;
    }

    int total_sum = 0;
    double start_time = MPI_Wtime();

    // Reducir las sumas parciales
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    if (rank == 0) {
        printf("\nLa suma de los %d primeros numeros naturales es: %d\n", n, total_sum);
        printf("Tiempo total de ejecución paralelo: %.6f segundos\n", end_time - start_time);
        printf("---------- Program End ----------\n");
    }

    MPI_Finalize();
    return 0;
}
