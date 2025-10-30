#!/usr/bin/env python3
"""
SymmetricMatrix Sequential - EntradaAleatoria (Python + numpy)
Genera una matriz aleatoria (o usa params.txt para tamaño) y comprueba si es simétrica.
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
n = 5
m = 5
# if params.txt exists in current directory, try to read n and m from it
params_path = os.path.join(os.path.dirname(__file__), '..', '..', 'params.txt')
if os.path.exists(params_path):
    try:
        with open(params_path, 'r') as f:
            parts = f.read().strip().split()
            if len(parts) >= 2:
                n = int(parts[0]); m = int(parts[1])
    except Exception:
        pass

if n != m:
    print('EntradaAleatoria: para simetría la matriz debe ser cuadrada; ajustando m = n')
    m = n

# generate random matrix
A = np.random.randint(0, 10, size=(n, m), dtype=int)
start = time.process_time()

symmetric = np.array_equal(A, A.T)
end = time.process_time()

print('Matriz generada (fila-major):')
print(' '.join(str(int(x)) for x in A.ravel()))
if symmetric:
    print('La matriz es simétrica')
else:
    print('La matriz NO es simétrica')
print(f"Tiempo (CPU): {end - start:.6f} segundos")
