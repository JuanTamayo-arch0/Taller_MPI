#!/usr/bin/env python3
"""
RemoveDuplicates sequential - EntradaAleatoria (Python)
Reads size from params.txt if present, otherwise uses a default. Generates a random vector,
removes duplicates preserving first occurrences (O(n^2) like the C version), and prints timing (CPU time).
"""
import random
import time
import sys

DEFAULT_SIZE = 10

def read_size(path='params.txt'):
    try:
        with open(path,'r') as f:
            data = f.read().strip().split()
            if data:
                return int(data[0])
    except Exception:
        return None
    return None

def remove_element(array, index, size):
    for i in range(index, size-1):
        array[i] = array[i+1]
    # size reduced by caller

if __name__ == '__main__':
    size = read_size() or DEFAULT_SIZE
    print('--------- RemoveDuplicates&Sequential (EntradaAleatoria - Python) --------')
    vec = [random.randint(0, 100) for _ in range(size)]
    print('\nEl vector generado es: [', ' '.join(map(str, vec)), ']')

    start = time.process_time()
    i = 0
    current_size = size
    while i < current_size:
        j = i + 1
        while j < current_size:
            if vec[i] == vec[j]:
                remove_element(vec, j, current_size)
                current_size -= 1
            else:
                j += 1
        i += 1
    elapsed = time.process_time() - start

    vec = vec[:current_size]
    print('\nEl vector sin duplicados es: [', ' '.join(map(str, vec)), ']')
    print(f"\nTiempo total de ejecución (CPU): {elapsed:.6f} segundos")
    print('---------- Program End ----------')
