#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

void swap(int* a, int* b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void merge(int* arr, int* temp, int left, int mid, int right) {
    int i = left, j = mid, k = left;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i < mid) temp[k++] = arr[i++];
    while (j < right) temp[k++] = arr[j++];
    for (i = left; i < right; i++) arr[i] = temp[i];
}

int main(int argc, char** argv) {
    int npr, pid;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &npr);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);

    int size;
    int* vector = NULL;
    double start, end;

    if (pid == 0) {
        printf("--------- SortVector&Parallel (MPI) --------\n");
        printf("Ingrese la cantidad de elementos del vector: ");
        scanf("%d", &size);

        vector = (int*)malloc(size * sizeof(int));
        srand(time(NULL));

        printf("\nEl vector generado: [ ");
        for (int i = 0; i < size; i++) {
            vector[i] = rand() % 101;
            printf("%d ", vector[i]);
        }
        printf("]\n");
    }

    // Broadcast del tamaño a todos
    MPI_Bcast(&size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int local_size = size / npr;
    int* local_arr = (int*)malloc(local_size * sizeof(int));

    // Distribuir los datos
    MPI_Scatter(vector, local_size, MPI_INT, local_arr, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    start = MPI_Wtime();
    quickSort(local_arr, 0, local_size - 1);
    end = MPI_Wtime();

    // Recolectar los resultados
    int* sorted = NULL;
    if (pid == 0) {
        sorted = (int*)malloc(size * sizeof(int));
    }

    MPI_Gather(local_arr, local_size, MPI_INT, sorted, local_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (pid == 0) {
        // Merge secuencial de los bloques ordenados
        int* temp = (int*)malloc(size * sizeof(int));
        for (int i = 1; i < npr; i++) {
            merge(sorted, temp, 0, i * local_size, (i + 1) * local_size);
        }

        printf("\nVector ordenado final: [ ");
        for (int i = 0; i < size; i++) {
            printf("%d ", sorted[i]);
        }
        printf("]\n");

        printf("\nTiempo total (MPI_Wtime): %f segundos\n", end - start);
        printf("-------------------------------------------\n");

        free(sorted);
        free(temp);
        free(vector);
    }

    free(local_arr);
    MPI_Finalize();
    return 0;
}