#!/usr/bin/env python3
"""
SumMatrix Sequential - EntradaAleatoria (Python + numpy)
Genera dos matrices aleatorias (o usa params.txt para tamaño) y muestra su suma.
"""
import sys
import os
try:
    import numpy as np
    import time
except Exception:
    print('This script requires numpy. Install with: pip install numpy')
    sys.exit(1)

# default size
n = 4
m = 4
params_path = os.path.join(os.path.dirname(__file__), '..', '..', 'params.txt')
if os.path.exists(params_path):
    try:
        with open(params_path, 'r') as f:
            parts = f.read().strip().split()
            if len(parts) >= 2:
                n = int(parts[0]); m = int(parts[1])
    except Exception:
        pass

A = np.random.randint(0, 10, size=(n, m), dtype=int)
B = np.random.randint(0, 10, size=(n, m), dtype=int)
start = time.process_time()
C = A + B
end = time.process_time()

print('Matriz A (fila-major):')
print(' '.join(str(int(x)) for x in A.ravel()))
print('Matriz B (fila-major):')
print(' '.join(str(int(x)) for x in B.ravel()))
print('Suma (fila-major):')
print(' '.join(str(int(x)) for x in C.ravel()))
print(f"Tiempo (CPU): {end - start:.6f} segundos")
