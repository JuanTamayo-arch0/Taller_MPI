#!/usr/bin/env python3
"""
SparseMatrix sequential - EntradaAleatoria (Python, numpy)
Reads n m from params.txt if present, otherwise uses defaults. Generates a random 0/1 matrix,
prints it, counts zeros and reports whether it's sparse (>50% zeros). Uses CPU timing (time.process_time()).
"""
import sys
import time
try:
    import numpy as np
except ImportError:
    print('numpy is required for this script. Install with: pip install numpy')
    sys.exit(1)

DEFAULT_N = 10
DEFAULT_M = 10

def read_dims(path='params.txt'):
    try:
        with open(path,'r') as f:
            toks = f.read().strip().split()
            if len(toks) >= 2:
                return int(toks[0]), int(toks[1])
    except Exception:
        pass
    return None, None

if __name__ == '__main__':
    n,m = read_dims()
    if n is None or m is None:
        n,m = DEFAULT_N, DEFAULT_M
    print('--------- SparseMatrix&Sequential (EntradaAleatoria - Python) --------')
    mat = np.random.randint(0,2,size=(n,m), dtype='i')
    print('\nLa matriz generada es:')
    for i in range(n):
        print(' '.join(str(int(x)) for x in mat[i]))

    start = time.process_time()
    zeros = int((mat == 0).sum())
    end = time.process_time()

    total = n*m
    print(f"\nCantidad total de ceros: {zeros} de {total} elementos")
    if zeros > total/2:
        print('La matriz es dispersa (sparse)')
    else:
        print('La matriz no es dispersa')
    print(f"Tiempo total de ejecución (CPU): {end - start:.6f} segundos")
    print('---------- Program End ----------')
