#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

/* -----------------------------------------------------------------------------
 * Programa: SumN&MPI
 * ------------------
 * Este programa calcula la suma de los primeros n números naturales de manera
 * paralela utilizando MPI. Cada proceso calcula una suma parcial de su rango
 * de números y luego se combinan mediante MPI_Reduce.
 * -----------------------------------------------------------------------------
 */

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv); // Inicializar entorno MPI

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Identificador del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &size); // Número total de procesos

    int n; // Cantidad de números a sumar

    // Proceso maestro obtiene n desde la línea de comandos
    if (rank == 0) {
        if (argc < 2) {
            fprintf(stderr, "Usage: mpirun -np <procs> %s <n>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        printf("--------- SumN&MPI (CommandLine) --------\n");
        n = atoi(argv[1]);
    }

    // Enviar n a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular cuántos números maneja cada proceso
    int numbers_per_proc = n / size;
    int remainder = n % size;

    // Determinar rango de números para este proceso
    int start = rank * numbers_per_proc + (rank < remainder ? rank : remainder) + 1;
    int end = start + numbers_per_proc - 1 + (rank < remainder ? 1 : 0);

    // Calcular suma parcial de cada proceso
    int local_sum = 0;
    for (int i = start; i <= end; i++) {
        local_sum += i;
    }

    int total_sum = 0;

    // Medir tiempo de ejecución paralelo
    MPI_Barrier(MPI_COMM_WORLD); // Sincronizar procesos
    double start_time = MPI_Wtime();

    // Reducir las sumas parciales a la suma total en el proceso maestro
    MPI_Reduce(&local_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double end_time = MPI_Wtime();

    // Proceso maestro imprime el resultado y tiempo
    if (rank == 0) {
        printf("\nLa suma de los %d primeros numeros naturales es: %d\n", n, total_sum);
        printf("Tiempo total de ejecución paralelo: %.6f segundos\n", end_time - start_time);
        printf("---------- Program End ----------\n");
    }

    MPI_Finalize(); // Finalizar entorno MPI
    return 0;
}
