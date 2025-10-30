#!/usr/bin/env python3
"""
RemoveDuplicates sequential - EntradaCommandLine (Python)
Usage: python remove_duplicates.py <size> <v0> <v1> ... <v(size-1)>
Implements the same O(n^2) removal algorithm as the C version and prints CPU timing.
"""
import sys
import time

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print(f"Usage: {sys.argv[0]} <size> <v0> <v1> ... <v(size-1)>")
        sys.exit(1)
    size = int(sys.argv[1])
    if len(sys.argv) < 2 + size:
        print(f"Usage: {sys.argv[0]} <size> <v0> <v1> ... <v(size-1)>")
        sys.exit(1)
    vec = [int(x) for x in sys.argv[2:2+size]]
    print('--------- RemoveDuplicates&Sequential (CommandLine - Python) --------')
    print('\nEl vector proporcionado es: [', ' '.join(map(str, vec)), ']')

    start = time.process_time()
    i = 0
    current_size = size
    while i < current_size:
        j = i + 1
        while j < current_size:
            if vec[i] == vec[j]:
                for k in range(j, current_size-1):
                    vec[k] = vec[k+1]
                current_size -= 1
            else:
                j += 1
        i += 1
    elapsed = time.process_time() - start

    vec = vec[:current_size]
    print('\nEl vector sin duplicados es: [', ' '.join(map(str, vec)), ']')
    print(f"\nTiempo total de ejecución (CPU): {elapsed:.6f} segundos")
    print('---------- Program End ----------')
