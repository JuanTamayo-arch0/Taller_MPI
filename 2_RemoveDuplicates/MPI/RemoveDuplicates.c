#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>

void remove_element(int *array, int index, int *size) {
    for (int i = index; i < *size - 1; i++)
        array[i] = array[i + 1];
    (*size)--;
}

void remove_duplicates_local(int *vector, int *size) {
    for (int i = 0; i < *size; i++) {
        for (int j = i + 1; j < *size; ) {
            if (vector[i] == vector[j]) {
                remove_element(vector, j, size);
            } else {
                j++;
            }
        }
    }
}

int main(int argc, char **argv) {
    int pid, npr;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);

    int size;
    int *vector = NULL;
    double start_time, end_time;

    if (pid == 0) {
        printf("--------- RemoveDuplicates&MPI --------\n");
        printf("Ingrese la cantidad de elementos del vector: ");
        scanf("%d", &size);

        vector = malloc(size * sizeof(int));
        srand(time(NULL));
        printf("\nEl vector elegido al azar es: [ ");
        for (int i = 0; i < size; i++) {
            vector[i] = rand() % 101;
            printf("%d ", vector[i]);
        }
        printf("]\n");
    }

    // Compartir el tamaño total
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular divisiones
    int *sendcounts = NULL;
    int *displs = NULL;
    int local_size;

    if (pid == 0) {
        sendcounts = malloc(npr * sizeof(int));
        displs = malloc(npr * sizeof(int));
        int base = size / npr;
        int rem = size % npr;
        int offset = 0;

        for (int i = 0; i < npr; i++) {
            sendcounts[i] = base + (i < rem ? 1 : 0);
            displs[i] = offset;
            offset += sendcounts[i];
        }
    }

    // Cada proceso recibe el tamaño de su parte
    MPI_Scatter(sendcounts, 1, MPI_INT, &local_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int *local_vec = malloc(local_size * sizeof(int));

    // Distribuir los datos
    MPI_Scatterv(vector, sendcounts, displs, MPI_INT, local_vec, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Tiempo de ejecución (solo local)
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    // Eliminar duplicados localmente
    remove_duplicates_local(local_vec, &local_size);

    // Recolectar tamaños resultantes
    int *recvcounts = NULL;
    int *recvdispls = NULL;
    if (pid == 0) {
        recvcounts = malloc(npr * sizeof(int));
    }
    MPI_Gather(&local_size, 1, MPI_INT, recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular desplazamientos para Gatherv final
    if (pid == 0) {
        recvdispls = malloc(npr * sizeof(int));
        recvdispls[0] = 0;
        for (int i = 1; i < npr; i++) {
            recvdispls[i] = recvdispls[i - 1] + recvcounts[i - 1];
        }
    }

    // Reunir todos los resultados locales
    int total_size = 0;
    if (pid == 0) {
        for (int i = 0; i < npr; i++)
            total_size += recvcounts[i];
        free(vector);
        vector = malloc(total_size * sizeof(int));
    }

    MPI_Gatherv(local_vec, local_size, MPI_INT, vector, recvcounts, recvdispls, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (pid == 0) {
        // Eliminar duplicados globalmente
        remove_duplicates_local(vector, &total_size);

        printf("\nEl vector sin duplicados es: [ ");
        for (int i = 0; i < total_size; i++) {
            printf("%d ", vector[i]);
        }
        printf("]\n");

        printf("\nTiempo total de ejecución (MPI_Wtime): %.6f segundos\n", end_time - start_time);
        printf("---------- Program End ----------\n");

        free(vector);
        free(sendcounts);
        free(displs);
        free(recvcounts);
        free(recvdispls);
    }

    free(local_vec);
    MPI_Finalize();
    return 0;
}
