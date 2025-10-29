#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
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

int main(){
	srand(time(NULL));	

	int size;

	printf("--------- SortVector&Sequential --------\n");

	printf("Ingrese la cantidad de elementos del vector: ");
	
	scanf("%d", &size);	

	int vector[size];

	printf("\nEl vector elegido al azar es: [ ");

	for(int i = 0; i<size;i++){
		vector[i] = rand() % (100 - 0 +1);
		printf("%d ",vector[i]);	
	}

	printf("]\n");

	printf("\nEl vector ordenado es: [ ");
	
	int SequentialTime = clock();

	quickSort(vector, 0, size - 1);
	SequentialTime = clock() - SequentialTime;

	for(int i =0;i<size;i++){
		printf("%d ", vector[i]);
	}
	
	printf("]\n");

	printf("\nY se demoro un total de %f segundos", (float)SequentialTime / CLOCKS_PER_SEC);
	
	printf("\n---------- Program End ----------");
}
