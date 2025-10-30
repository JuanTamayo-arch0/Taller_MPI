#!/usr/bin/env python3
"""
SymmetricMatrix MPI - EntradaAleatoria (Python, numpy + mpi4py)
Root genera una matriz aleatoria (o usa params.txt para tamaño), broadcast y cada rank verifica filas; reduce AND final.
"""
from mpi4py import MPI
import os
import sys
try:
    import numpy as np
except Exception:
    if MPI.COMM_WORLD.Get_rank() == 0:
        print('numpy is required. Install with: pip install numpy')
    MPI.COMM_WORLD.Abort(1)

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

# default size
n = 5
m = 5
params_path = os.path.join(os.path.dirname(__file__), '..', '..', 'params.txt')
if rank == 0 and os.path.exists(params_path):
    try:
        with open(params_path, 'r') as f:
            parts = f.read().strip().split()
            if len(parts) >= 2:
                n = int(parts[0]); m = int(parts[1])
    except Exception:
        pass

# broadcast sizes
n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)
if n != m:
    # force square for symmetric check
    m = n

# root generates matrix
if rank == 0:
    A = np.random.randint(0, 10, size=(n, n), dtype='i')
    A_flat = A.ravel().astype('i')
else:
    A_flat = None

# ensure all ranks have a receiver buffer for Bcast
if rank != 0:
    A_flat = np.empty(n * n, dtype='i')
comm.Bcast(A_flat, root=0)
A = A_flat.reshape((n, n))

comm.Barrier()
start = MPI.Wtime()
# compute rows assigned to this rank
rows_per = n // size
rem = n % size
start_row = rank * rows_per + min(rank, rem)
my_rows = rows_per + (1 if rank < rem else 0)
end_row = start_row + my_rows

local_ok = True
for i in range(start_row, end_row):
    # compare row i with column i
    if not np.array_equal(A[i, :], A[:, i]):
        local_ok = False
        break

# reduce with logical AND across ranks
is_sym = comm.allreduce(local_ok, op=MPI.LAND)
end = MPI.Wtime()

if rank == 0:
    print('Matriz generada (fila-major):')
    print(' '.join(str(int(x)) for x in A.ravel()))
    if is_sym:
        print('La matriz es simétrica')
    else:
        print('La matriz NO es simétrica')
    print(f"Tiempo (MPI_Wtime): {end - start:.6f} segundos")
