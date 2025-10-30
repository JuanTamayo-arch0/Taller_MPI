#!/usr/bin/env python3
"""
SymmetricMatrix Sequential - EntradaCommandLine (Python + numpy)
Usage: python symmetric_matrix.py <n> <m> <a00> <a01> ...
Checks whether the provided matrix is symmetric (requires square matrix n==m).
"""
import sys
try:
    import numpy as np
    import time
except Exception as e:
    print('This script requires numpy. Install with: pip install numpy')
    sys.exit(1)

if len(sys.argv) < 3:
    print(f"Usage: {sys.argv[0]} <n> <m> <a00> <a01> ... <a{sys.argv[1] if len(sys.argv)>1 else 'n'}>")
    sys.exit(1)

n = int(sys.argv[1])
m = int(sys.argv[2])
expected = n * m
if len(sys.argv) < 3 + expected:
    print(f"Expected {expected} matrix elements after n and m")
    sys.exit(1)

vals = [int(x) for x in sys.argv[3:3+expected]]
A = np.array(vals, dtype=int).reshape((n, m))
start = time.process_time()

if n != m:
    print('La matriz no es cuadrada; no puede ser simétrica')
    symmetric = False
else:
    symmetric = np.array_equal(A, A.T)

end = time.process_time()

if symmetric:
    print('La matriz es simétrica')
else:
    print('La matriz NO es simétrica')
print(f"Tiempo (CPU): {end - start:.6f} segundos")
