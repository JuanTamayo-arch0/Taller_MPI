#!/usr/bin/env python3
"""
SumMatrix Sequential - EntradaCommandLine (Python + numpy)
Usage: python sum_matrix.py <n> <m> <a00>...<a(n-1)(m-1)> <b00>...<b(n-1)(m-1)>
Reads two matrices A and B and prints their sum.
"""
import sys
try:
    import numpy as np
    import time
except Exception:
    print('This script requires numpy. Install with: pip install numpy')
    sys.exit(1)

if len(sys.argv) < 3:
    print(f"Usage: {sys.argv[0]} <n> <m> <a00>... <b00>...")
    sys.exit(1)

n = int(sys.argv[1]); m = int(sys.argv[2])
expected = n * m
if len(sys.argv) < 3 + expected * 2:
    print(f"Expected {expected*2} matrix elements after n and m (A then B)")
    sys.exit(1)

vals = [int(x) for x in sys.argv[3:3+expected*2]]
A = np.array(vals[:expected], dtype=int).reshape((n, m))
B = np.array(vals[expected:], dtype=int).reshape((n, m))
start = time.process_time()
C = A + B
end = time.process_time()

# print result in row-major single line
print('Suma de matrices (fila-major):')
print(' '.join(str(int(x)) for x in C.ravel()))
print(f"Tiempo (CPU): {end - start:.6f} segundos")
