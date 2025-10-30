#!/usr/bin/env python3
"""
SumMatrix MPI - EntradaAleatoria (Python, numpy + mpi4py)
Root genera dos matrices aleatorias (o usa params.txt para tamaño), Scatterv filas A y B, suma local y Gatherv resultado.
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
n = 4
m = 4
params_path = os.path.join(os.path.dirname(__file__), '..', '..', 'params.txt')
if rank == 0 and os.path.exists(params_path):
    try:
        with open(params_path, 'r') as f:
            parts = f.read().strip().split()
            if len(parts) >= 2:
                n = int(parts[0]); m = int(parts[1])
    except Exception:
        pass

n = comm.bcast(n, root=0)
m = comm.bcast(m, root=0)

rows_per = n // size
rem = n % size
my_rows = rows_per + (1 if rank < rem else 0)
local_buf_A = np.empty(my_rows * m, dtype='i')
local_buf_B = np.empty(my_rows * m, dtype='i')

if rank == 0:
    A = np.random.randint(0, 10, size=(n, m), dtype='i')
    B = np.random.randint(0, 10, size=(n, m), dtype='i')
    A_flat = A.ravel()
    B_flat = B.ravel()
    sendcounts = np.array([(rows_per + (1 if i < rem else 0)) * m for i in range(size)], dtype='i')
    displs = np.array([int(sum(sendcounts[:i])) for i in range(size)], dtype='i')
else:
    A_flat = None; B_flat = None; sendcounts = None; displs = None

comm.Scatterv([A_flat, sendcounts, displs, MPI.INT] if rank == 0 else [None, None, None, MPI.INT], local_buf_A, root=0)
comm.Scatterv([B_flat, sendcounts, displs, MPI.INT] if rank == 0 else [None, None, None, MPI.INT], local_buf_B, root=0)

comm.Barrier()
start = MPI.Wtime()
local_sum = local_buf_A + local_buf_B
# gather
if rank == 0:
    result_flat = np.empty(n*m, dtype='i')
else:
    result_flat = None
comm.Gatherv(local_sum, [result_flat, sendcounts, displs, MPI.INT], root=0)
end = MPI.Wtime()

if rank == 0:
    print('Matriz A (fila-major):')
    print(' '.join(str(int(x)) for x in A_flat))
    print('Matriz B (fila-major):')
    print(' '.join(str(int(x)) for x in B_flat))
    print('Suma (fila-major):')
    print(' '.join(str(int(x)) for x in result_flat))
    print(f"Tiempo (MPI_Wtime): {end - start:.6f} segundos")
