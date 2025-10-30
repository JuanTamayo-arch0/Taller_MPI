#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

/* -----------------------------------------------------------------------------
 * Programa: SparseMatrix&MPI
 * --------------------------
 * Determina si una matriz generada aleatoriamente es dispersa (sparse).
 * Una matriz se considera sparse si la cantidad de ceros > 50% de sus elementos.
 * 
 * Paraleliza el conteo de ceros usando MPI.
 * -----------------------------------------------------------------------------
 */
int main(int argc, char **argv) {
    int pid, npr;  // pid: identificador del proceso, npr: número de procesos
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);

    int n, m;             // filas y columnas de la matriz
    int *matrix = NULL;   // matriz almacenada en 1D (fila mayor)
    int local_zeros = 0;  // ceros contados localmente
    int total_zeros = 0;  // ceros totales (reducido en proceso 0)
    double start_time, end_time;

    // -------------------------------
    // Proceso maestro: pedir dimensiones y generar matriz
    // -------------------------------
    if (pid == 0) {
        printf("--------- SparseMatrix&Sequential (Parametros) --------\n");

        FILE* f = fopen("params.txt", "r");
        if (!f) {
            fprintf(stderr, "params.txt no encontrado. Coloque un archivo con n y m en esta carpeta.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        if (fscanf(f, "%d %d", &n, &m) != 2) {
            fprintf(stderr, "Formato de params.txt inválido. Debe contener dos enteros: n m.\n");
            fclose(f);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        // Reservar memoria para la matriz como vector 1D y leer elementos desde params.txt
        matrix = malloc(n * m * sizeof(int));
        if (!matrix) {
            perror("Error al asignar memoria");
            fclose(f);
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }

        printf("\nLa matriz leida de params.txt es:\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (fscanf(f, "%d", &matrix[i * m + j]) != 1) {
                    fprintf(stderr, "params.txt no contiene suficientes elementos (se esperaban %d).\n", n * m);
                    fclose(f);
                    free(matrix);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
                printf("%d ", matrix[i * m + j]);
            }
            printf("\n");
        }
        fclose(f);
    }

    // -------------------------------
    // Difundir dimensiones a todos los procesos
    // -------------------------------
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int total_elements = n * m;

    // -------------------------------
    // Calcular cantidad de elementos por proceso
    // -------------------------------
    int base = total_elements / npr;
    int rem = total_elements % npr;
    int local_size = base + (pid < rem ? 1 : 0);
    int offset = pid * base + (pid < rem ? pid : rem);

    // -------------------------------
    // Cada proceso cuenta los ceros en su porción
    // -------------------------------
    start_time = MPI_Wtime();
    for (int i = 0; i < local_size; i++) {
        int idx = offset + i;
        int val;
        if (pid == 0) {
            val = matrix[idx];
        }
        // Difundir el valor a todos los procesos no maestros
        MPI_Bcast(&val, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (val == 0)
            local_zeros++;
    }

    // -------------------------------
    // Reducir conteo local a total en el proceso 0
    // -------------------------------
    MPI_Reduce(&local_zeros, &total_zeros, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    // -------------------------------
    // Proceso maestro muestra resultados
    // -------------------------------
    if (pid == 0) {
        if (total_zeros > total_elements / 2)
            printf("\nLa matriz es dispersa (sparse)\n");
        else
            printf("\nLa matriz no es dispersa\n");

        printf("Cantidad de ceros: %d de %d elementos\n", total_zeros, total_elements);
        printf("Tiempo total de ejecución (MPI_Wtime): %.6f segundos\n", end_time - start_time);

        free(matrix);
        printf("---------- Program End ----------\n");
    }

    MPI_Finalize();
    return 0;
}
