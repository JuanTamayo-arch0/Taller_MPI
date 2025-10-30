#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <mpi.h>

/* -----------------------------------------------------------------------------
 * Función: remove_element
 * -----------------------
 * Elimina un elemento de un arreglo desplazando los elementos siguientes.
 *
 * array : Arreglo de enteros.
 * index : Posición del elemento a eliminar.
 * size  : Puntero al tamaño actual del arreglo (se decrementa).
 * -----------------------------------------------------------------------------
 */
void remove_element(int *array, int index, int *size) {
    for (int i = index; i < *size - 1; i++)
        array[i] = array[i + 1];  // Desplazar elementos hacia la izquierda
    (*size)--;                     // Reducir tamaño lógico del arreglo
}

/* -----------------------------------------------------------------------------
 * Función: remove_duplicates_local
 * --------------------------------
 * Elimina los duplicados dentro de un arreglo dado de forma secuencial.
 *
 * vector : Arreglo de enteros.
 * size   : Puntero al tamaño actual del arreglo.
 * -----------------------------------------------------------------------------
 */
void remove_duplicates_local(int *vector, int *size) {
    for (int i = 0; i < *size; i++) {
        for (int j = i + 1; j < *size; ) {
            if (vector[i] == vector[j]) {
                remove_element(vector, j, size);  // Eliminar duplicado
            } else {
                j++;  // Avanzar si no hay duplicado
            }
        }
    }
}

/* -----------------------------------------------------------------------------
 * Función principal
 * -----------------
 * Implementa la eliminación de duplicados de forma paralela usando MPI.
 * Flujo:
 * 1. Proceso maestro (pid 0) genera el vector aleatorio.
 * 2. Se difunde el tamaño del vector a todos los procesos.
 * 3. Se calcula la distribución de los elementos (sendcounts y displs).
 * 4. Cada proceso recibe su subvector (MPI_Scatterv).
 * 5. Cada proceso elimina duplicados localmente.
 * 6. Se recopilan los tamaños de los subvectores procesados.
 * 7. Se realiza MPI_Gatherv para unir los resultados en el proceso maestro.
 * 8. El proceso maestro elimina duplicados globales y muestra resultados.
 * -----------------------------------------------------------------------------
 */
int main(int argc, char **argv) {
    int pid, npr;  // pid = identificador del proceso, npr = número de procesos
    MPI_Init(&argc, &argv);                 // Inicializar entorno MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);    // Obtener el ID del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &npr);    // Obtener el número total de procesos

    int size;         // Tamaño total del vector
    int *vector = NULL;
    double start_time, end_time;  // Medición de tiempo

    /* -----------------------------
     * Proceso maestro: genera vector aleatorio
     * -----------------------------
     */
    if (pid == 0) {
            printf("--------- RemoveDuplicates&MPI (Parametros) --------\n");

            FILE* f = fopen("params.txt", "r");
            if (!f) {
                fprintf(stderr, "params.txt no encontrado. Coloque un archivo con el tamaño y los elementos en esta carpeta.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            if (fscanf(f, "%d", &size) != 1) {
                fprintf(stderr, "Formato de params.txt inválido. Debe contener un entero (size).\n");
                fclose(f);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            // Reservar memoria para el vector principal
            vector = (int*)malloc(size * sizeof(int));
            if (!vector) {
                perror("Error al asignar memoria");
                fclose(f);
                MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
            }

            // Leer elementos desde params.txt
            printf("\nEl vector leido de params.txt es: [ ");
            for (int i = 0; i < size; i++) {
                if (fscanf(f, "%d", &vector[i]) != 1) {
                    fprintf(stderr, "params.txt no contiene suficientes elementos (se esperaban %d).\n", size);
                    fclose(f);
                    MPI_Abort(MPI_COMM_WORLD, 1);
                }
                printf("%d ", vector[i]);
            }
            printf("]\n");
            fclose(f);
    }

    /* -----------------------------------------------------
     * Difundir el tamaño total a todos los procesos
     * -----------------------------------------------------
     */
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* -----------------------------------------------------
     * Calcular cómo se repartirán los elementos entre procesos
     * sendcounts: cantidad de elementos por proceso
     * displs: desplazamientos iniciales de cada subvector
     * -----------------------------------------------------
     */
    int *sendcounts = NULL;
    int *displs = NULL;
    int local_size;

    if (pid == 0) {
        sendcounts = malloc(npr * sizeof(int));
        displs = malloc(npr * sizeof(int));

        int base = size / npr;          // Tamaño mínimo por proceso
        int rem = size % npr;           // Elementos sobrantes
        int offset = 0;

        for (int i = 0; i < npr; i++) {
            sendcounts[i] = base + (i < rem ? 1 : 0);  // Distribuir sobrante
            displs[i] = offset;                        // Posición inicial
            offset += sendcounts[i];
        }
    }

    /* -----------------------------
     * Cada proceso recibe el tamaño de su porción
     * -----------------------------
     */
    MPI_Scatter(sendcounts, 1, MPI_INT, &local_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Reservar memoria para el subvector local
    int *local_vec = malloc(local_size * sizeof(int));

    /* -----------------------------
     * Distribuir las porciones del vector original
     * -----------------------------
     */
    MPI_Scatterv(vector, sendcounts, displs, MPI_INT,
                 local_vec, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    /* -----------------------------
     * Sincronizar todos los procesos antes de medir tiempo
     * -----------------------------
     */
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    /* -----------------------------
     * Cada proceso elimina duplicados en su subvector
     * -----------------------------
     */
    remove_duplicates_local(local_vec, &local_size);

    /* -----------------------------
     * Recolectar tamaños de vectores procesados
     * -----------------------------
     */
    int *recvcounts = NULL;
    int *recvdispls = NULL;
    if (pid == 0) {
        recvcounts = malloc(npr * sizeof(int));
    }

    MPI_Gather(&local_size, 1, MPI_INT,
               recvcounts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular desplazamientos para Gatherv
    if (pid == 0) {
        recvdispls = malloc(npr * sizeof(int));
        recvdispls[0] = 0;
        for (int i = 1; i < npr; i++) {
            recvdispls[i] = recvdispls[i - 1] + recvcounts[i - 1];
        }
    }

    /* -----------------------------
     * Reunir todos los vectores locales en el maestro
     * -----------------------------
     */
    int total_size = 0;
    if (pid == 0) {
        for (int i = 0; i < npr; i++)
            total_size += recvcounts[i];
        free(vector);  // liberar el vector original
        vector = malloc(total_size * sizeof(int));
    }

    MPI_Gatherv(local_vec, local_size, MPI_INT,
                vector, recvcounts, recvdispls, MPI_INT, 0, MPI_COMM_WORLD);

    /* -----------------------------
     * Detener cronómetro
     * -----------------------------
     */
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    /* -----------------------------
     * Proceso maestro: eliminar duplicados globalmente
     * -----------------------------
     */
    if (pid == 0) {
        remove_duplicates_local(vector, &total_size);

        printf("\nEl vector sin duplicados es: [ ");
        for (int i = 0; i < total_size; i++) {
            printf("%d ", vector[i]);
        }
        printf("]\n");

        printf("\nTiempo total de ejecución (MPI_Wtime): %.6f segundos\n", end_time - start_time);
        printf("---------- Program End ----------\n");

        // Liberar memoria del maestro
        if (sendcounts) free(sendcounts);
        if (displs) free(displs);
        if (recvcounts) free(recvcounts);
        if (recvdispls) free(recvdispls);
        free(vector);
    }

    // Liberar memoria local y finalizar MPI
    free(local_vec);
    MPI_Finalize();
    return 0;
}
