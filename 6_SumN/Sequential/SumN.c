#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    int n;
    int sum = 0;

    printf("--------- SumN&Sequential --------\n");
    printf("Ingrese la cantidad de los primeros numeros naturales a sumar: ");
    scanf("%d", &n);

    clock_t start = clock();

    printf("\nLos primeros %d numeros naturales son:\n", n);
    for (int i = 1; i <= n; i++) {
        sum += i;
        printf("%d ", i);
    }

    clock_t end = clock();

    printf("\n\nLa suma de los %d primeros numeros naturales es: %d\n", n, sum);
    printf("Tiempo total de ejecución: %.6f segundos\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("---------- Program End ----------\n");
    return 0;
}
