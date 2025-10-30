#!/usr/bin/env python3
"""
SumN sequential - EntradaAleatoria (Python)
Reads n from params.txt if present, otherwise uses a default; prints numbers 1..n and their sum and CPU time.
"""
import time
import sys
import random

DEFAULT_N = 10

def read_n(path='params.txt'):
    try:
        with open(path,'r') as f:
            data = f.read().strip().split()
            if data:
                return int(data[0])
    except Exception:
        return None
    return None

if __name__ == '__main__':
    n = read_n() or DEFAULT_N
    print('--------- SumN&Sequential (EntradaAleatoria - Python) --------')
    start = time.process_time()
    s = 0
    print('\nLos primeros', n, 'numeros naturales son:')
    for i in range(1, n+1):
        s += i
        print(i, end=' ')
    end = time.process_time()
    print('\n\nLa suma es:', s)
    print(f'Tiempo total de ejecución (CPU): {end - start:.6f} segundos')
    print('---------- Program End ----------')
