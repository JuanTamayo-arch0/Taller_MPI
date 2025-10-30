#!/usr/bin/env python3
"""
SortVector sequential - EntradaCommandLine (Python)
Usage: python sort_vector.py <size> <v0> <v1> ... <v(size-1)>
Mimics the C CommandLine version: reads elements from argv, sorts and prints.
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
    print("--------- SortVector&Sequential (CommandLine - Python) --------")
    print('\nEl vector proporcionado es: [', ' '.join(map(str, vec)), ']')
    start = time.process_time()
    vec.sort()
    elapsed = time.process_time() - start
    print('\nEl vector ordenado es: [', ' '.join(map(str, vec)), ']')
    print(f"\nTiempo total de ejecución (CPU): {elapsed:.6f} segundos")
    print('---------- Program End ----------')
