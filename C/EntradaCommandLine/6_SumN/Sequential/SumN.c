#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* -----------------------------------------------------------------------------
 * Programa: SumN&Sequential
 * -------------------------
 * Este programa calcula la suma de los primeros n números naturales de manera
 * secuencial y muestra los números junto con su suma total. También mide el
 * tiempo de ejecución de la operación.
 * -----------------------------------------------------------------------------
 */

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]); // Cantidad de números naturales a sumar
    int sum = 0; // Variable acumuladora para la suma

    printf("--------- SumN&Sequential (CommandLine) --------\n");

    // Iniciar medición de tiempo
    clock_t start = clock();

    // Mostrar los números y calcular la suma
    printf("\nLos primeros %d numeros naturales son:\n", n);
    for (int i = 1; i <= n; i++) {
        sum += i;      // Acumular suma
        printf("%d ", i); // Imprimir número actual
    }

    // Finalizar medición de tiempo
    clock_t end = clock();

    // Mostrar el resultado final y tiempo de ejecución
    printf("\n\nLa suma de los %d primeros numeros naturales es: %d\n", n, sum);
    printf("Tiempo total de ejecución: %.6f segundos\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    printf("---------- Program End ----------\n");
    return 0;
}
