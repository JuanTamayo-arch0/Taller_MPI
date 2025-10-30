#!/usr/bin/env python3
"""
SparseMatrix sequential - EntradaCommandLine (Python, numpy)
Usage: python sparse_matrix.py <n> <m> <a00> <a01> ... <a(n-1)(m-1)>
Reads matrix elements from argv in row-major order, counts zeros and reports sparse/not-sparse.
"""
import sys
import time
try:
    import numpy as np
except ImportError:
    print('numpy is required. Install with: pip install numpy')
    sys.exit(1)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <n> <m> <a00> <a01> ... <a(n-1)(m-1)>")
        sys.exit(1)
    n = int(sys.argv[1]); m = int(sys.argv[2])
    expected = n*m
    if len(sys.argv) < 3 + expected:
        print(f"Expected {expected} matrix elements after n and m")
        sys.exit(1)
    vals = [int(x) for x in sys.argv[3:3+expected]]
    mat = np.array(vals, dtype='i').reshape((n,m))
    print('--------- SparseMatrix&Sequential (CommandLine - Python) --------')
    print('\nLa matriz proporcionada es:')
    for i in range(n): print(' '.join(str(int(x)) for x in mat[i]))

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
